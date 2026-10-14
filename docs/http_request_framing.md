# Incremental HTTP request framing

Issue [#5](https://github.com/carolinapapes/webserv42/issues/5) adds the C++98
`RequestParser` in `src/http/request/request_parser.hpp`. Each instance owns its
input, parsing state, headers, and decoded body. It has no socket or epoll
dependency and shares no mutable request state with other instances.

## Feeding and consuming requests

```cpp
#include <request_parser.hpp>

RequestParserLimits limits;
limits.maxBodyBytes = 1024; // Effective limit chosen by the caller.
RequestParser parser(limits);

parser.feed("POST / HTTP/1.1\r\nHost: example.com\r\nContent-Length: 3\r\n\r\na");
// INCOMPLETE: the declared body still needs two bytes.
parser.feed("bcGET /next HTTP/1.1\r\nHost: example.com\r\n\r\n");
while (parser.result().status == RequestParseResult::COMPLETE)
{
    Request completed = parser.result().request;
    // Dispatch or save completed before advancing.
    parser.next();
}
```

`feed(bytes, endOfInput = false)` copies the supplied bytes and returns
COMPLETE, INCOMPLETE, or ERROR. `result()` returns the current result by const
reference. Only COMPLETE is ready for dispatch. Incomplete or rejected fields
are partial state and must not be used as a completed request.

`pendingData()` exposes the owned unconsumed input by const reference. At
completion it contains only bytes following the current message. While parsing
an incomplete message it can also contain an unfinished line or chunk delimiter.
It never includes body bytes already consumed by the parser.

`next()` advances only a COMPLETE parser, clears the current request, and parses
any queued bytes. With no queued bytes it returns INCOMPLETE without inventing
another request. Feeding a COMPLETE parser queues bytes while preserving its
current result, subject to the pending-input limit.

Use `feed("", true)` to signal end of input for an unfinished message. Truncated
headers, fixed bodies, chunk bodies, and trailers then report 400. End-of-input
also applies to requests already queued behind a completed request. No additional
input should be supplied after signaling end of input; use `reset()` for a new
input stream.

ERROR is terminal until `reset()`. The parser discards pending bytes after an
error and never scans ahead for a later request boundary. The connection owner
must handle the HTTP error and close the connection. `reset()` discards the
current result, partial body, queued data, and end-of-input flag while preserving
the configured limits. It is not the way to advance a successful pipeline.

References returned by `result()` and `pendingData()` must not be held across
mutating calls. Copy the result or Request to retain a snapshot; copies own their
data independently of input strings and later parser reuse.

## Body boundaries and decoding

- Without Content-Length or Transfer-Encoding, the request body is empty.
- Validated Content-Length requires exactly the declared number of octets.
  A partial body is INCOMPLETE; later bytes remain available for the next request.
- Supported Transfer-Encoding is chunked. Hexadecimal sizes, data CRLFs, the
  terminating zero chunk, trailer lines, and the final CRLF are parsed across
  arbitrary input boundaries. The Request body contains decoded binary octets.
- Chunk extensions accept token and quoted-string syntax, including escaped
  quotes, optional protocol whitespace, and opaque quoted bytes. Valid unknown
  extensions are ignored. Malformed sizes, extensions, and delimiters report 400.
- Trailer syntax is checked and accepted trailers are discarded. They are never
  merged into the original headers. Host, Content-Length, Transfer-Encoding,
  Connection, Trailer, TE, Authorization, Proxy-Authorization, Content-Encoding,
  Content-Type, and Content-Range are rejected in trailers with 400.

Original headers, including Transfer-Encoding and repeated fields, remain on the
Request. They describe the received message; its body has already been decoded.
The parser does not normalize headers for forwarding to another HTTP recipient.
Unsupported transfer-coding chains still report 501. Ambiguous TE/CL and
conflicting Content-Length fields still report 400 under issue #4's policies.

## Resource limits

Pass a `RequestParserLimits` value to the constructor. These are byte ceilings,
not timeout policies. The caller chooses the effective body ceiling from its
configuration before parsing; assigning zero permits empty bodies only.
Connecting server selection and configuration to parser instances belongs to
the connection-management integration work.

| Setting | Default | Accounting | Error |
| --- | --- | --- | --- |
| `maxRequestLineBytes` | 8192 | Request line including CRLF | 414 |
| `maxHeaderBytes` | 65536 | Header and trailer lines, including CRLF and each section's empty terminator | 431 |
| `maxBodyBytes` | 1048576 | Fixed or decoded body bytes | 413 |
| `maxPendingBytes` | 2097152 | Unconsumed input, including queued requests | 413 |
| `maxChunkLineBytes` | 8192 | One chunk-size/extension line including CRLF | 413 |
| `maxChunkMetadataBytes` | 65536 | All chunk-size lines and chunk-data CRLFs in one request | 413 |

The pending-input limit is checked before copying a feed. Feed smaller fragments
when a message is larger than that limit. Consumed body bytes leave the pending
buffer, so a body can exceed the pending-input limit while remaining within the
body limit. Queued requests are bounded even while the caller holds a completed
result. Syntax lines are scanned incrementally, without rescanning an entire
accumulated request on every feed.

The existing field-name/value bounds of 256/8192 bytes and INT_MAX Content-Length
numeric bound remain in force. Hexadecimal chunk-size overflow is detected before
multiplication and reports 413. Declared fixed and chunked sizes that exceed the
remaining body allowance are rejected before their payload arrives.

## Whole-buffer compatibility

`parseRequest(raw, endOfInput)` and `validateRequest(raw)` now use this framing
implementation. They return the first framed request; `validateRequest` treats
its input as final. A final incomplete body reports 400. Chunked bodies are
decoded, and bytes beyond Content-Length are no longer appended to the body.
Use `RequestParser` to preserve state and queued bytes across calls.

EpollConnectionManager still calls the whole-buffer wrapper and has not been
converted to one parser per connection. This issue provides parser-level
correctness; fragmented network dispatch, persistent connections, routing, and
CGI integration are not established by its unit tests.

## Docker validation for issue #5

Validated in the Ubuntu 24.04 Linux development image with
`-Wall -Wextra -Werror -std=c++98`, AddressSanitizer, and
`ASAN_OPTIONS=detect_leaks=1`.

| Check | Result |
| --- | --- |
| `make fclean`, `make re`, `make` | Pass |
| `make -j4 test` | Pass: all 14 C++ suites and all 8 Python runner tests |
| Incremental framing suite | Pass: 10,564 checks |
| Parsing results and shared error table | Pass: 521 checks covering all 39 error IDs |
| Existing request, scanner, header storage, and field-validation suites | Pass |
| `make test-http-regressions` | Pass: all 32 original checks |
| Live smoke: static GET, lowercase Host, empty/repeated fields, identical lengths, redirect, teapot response, CGI | Pass: 8 probes |
| Live duplicate-Host error response | Fail: server exits without an HTTP response; AddressSanitizer reports a use-after-free in `ServerManager::~ServerManager` |
| Remaining 6 malformed-request response probes | Not run after that server failure |

No AddressSanitizer or LeakSanitizer diagnostics appeared in the unit tests.
The initial smoke readiness check opened an empty connection and triggered the
same server crash before the static GET. Waiting for the startup log instead
allowed the eight positive probes above to run before the duplicate-Host failure.
The unchanged merged baseline, commit
`cf54f34f255212d73a569f12a09907db6670a705`, was built and checked separately
in Docker: the same eight positive probes passed and duplicate Host reproduced
the same ownership failure. Server ownership remains outside issue #5;
these results do not establish successful live malformed-request responses or
incremental network dispatch.

## References

- [RFC 9112 §6.3](https://www.rfc-editor.org/rfc/rfc9112.html#section-6.3): message body boundaries.
- [RFC 9112 §7.1](https://www.rfc-editor.org/rfc/rfc9112.html#section-7.1): chunked syntax, extensions, and trailers.
- [RFC 9110 §6.5](https://www.rfc-editor.org/rfc/rfc9110.html#section-6.5): trailer-field handling.
