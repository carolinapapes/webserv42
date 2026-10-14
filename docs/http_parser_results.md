# Scanner and explicit parsing results

Issue [#3](https://github.com/carolinapapes/webserv42/issues/3) fixes scanner
position handling and removes validation's dependency on a static active
Request. The implementation remains C++98 and uses no additional dependencies.

## Request parsing

Include `schemas.hpp` and call:

```cpp
RequestParseResult parseRequest(const std::string& raw, bool endOfInput = false);
```

The returned value owns a Request and a diagnostic string. `status` is one of:

| Status | Meaning | HTTP error code |
| --- | --- | --- |
| `RequestParseResult::COMPLETE` | The request line and headers passed the current validators; available body bytes were copied | 0 |
| `RequestParseResult::INCOMPLETE` | More bytes are needed to finish the request line or headers | 0 |
| `RequestParseResult::ERROR` | Syntax or validation failed, or final input ended before headers were complete | `getErrorCode()` |

`message` contains the reason for ERROR and is empty otherwise. The same code
is stored in `request.getErrorCode()` for compatibility with the Request model.
The parser returns diagnostics instead of printing them. Fields in an incomplete
or rejected Request are partial state and should not be dispatched.

```cpp
#include <schemas.hpp>

RequestParseResult partial = parseRequest("GET / HTTP/1.1\r");
// partial.status == RequestParseResult::INCOMPLETE

RequestParseResult complete = parseRequest(
    "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
// complete.status == RequestParseResult::COMPLETE
// complete.request owns the parsed fields; complete.getErrorCode() == 0

RequestParseResult truncated = parseRequest("GET / HTTP/1.1\r", true);
// truncated.status == RequestParseResult::ERROR
// truncated.getErrorCode() == 400
```

This is a stateless entry point. For unfinished input, retain the original bytes
and retry with the entire accumulated buffer. Separate calls and returned
results share no mutable Request state. They can be interleaved, copied, or
destroyed independently, and results remain valid after their input string is
changed or destroyed.

An unfinished CRLF is INCOMPLETE unless `endOfInput` is true. A bare LF or a CR
followed by another character is already invalid and reports 400. Missing
components in a terminated request line and malformed headers also report 400.
Unsupported methods report 501 and unsupported versions report 505. Issue #4
adds [header and request-line validation](http_header_validation.md), including
400 for malformed method/version syntax and 505 for HTTP/2.0 request lines.

The compatibility function `validateRequest(raw)` calls `parseRequest(raw, true)`
and returns its Request. This preserves the whole-buffer interface used by the
existing connection manager; it treats truncated final input as 400. No socket,
epoll, CGI, or routing code is changed in issue #3.

## Scope of completion

COMPLETE currently describes request-line/header parsing. Body bytes remain the
entire input remainder, matching the previous parser. It does not yet guarantee
that Content-Length bytes have arrived, decode chunked bodies, split pipelined
requests, or retain unconsumed input. Those are issue #5 responsibilities.
The feed API and its connection-manager integration contract follow in issues
#5 and #6; this entry point does not replace that work.

## Scanner extraction

`ReqScanner` owns a copy of its input and a position starting at zero:

```cpp
#include <req_scanner.hpp>

ReqScanner scanner("key::value");
std::string field;
bool found = scanner.getField(":", field); // true, field == "key"
found = scanner.getField(":", field);      // true, field is empty
std::string rest = scanner.getRemaining(); // "value", now at end
```

`getField(delimiter, field)` returns true only when it finds the entire delimiter.
An empty extracted field is a successful result. If the delimiter is missing or
partial, it returns false, clears `field`, and leaves the position unchanged.
It never adds a delimiter length to `std::string::npos`. Retrying another
delimiter therefore examines the same bytes. The delimiter and output arguments
may refer to the same string without changing how many input bytes are consumed.

`getRemaining()` explicitly consumes and returns the remaining bytes, including
NUL bytes. At the end it returns an empty string; `isEndPos()` reports whether all
bytes have been consumed. Empty input starts at the end. An empty delimiter is a
caller error (`std::invalid_argument`) and leaves the position unchanged. NUL can
still be used as a real delimiter with `std::string(1, '\0')`; it has no special
end-of-input meaning.

## Explicit validator errors

`RequestParseError` carries an HTTP code and a reason from the shared error table
in `request_parse_error.cpp`. Schema validation and the
URI, Host, and Content-Length validators throw it for detected failures. The
parser catches this type and constructs an ERROR result. Missing Host now also
reports 400 rather than throwing an untyped error while leaving Request code 0.

Request retains its existing data fields, copy operations, getters, and setters.
Its constructors and validators no longer register or mutate a global Request.
There is no `activeRequest`, `setActiveRequest`, `setActiveError`, or `EOF_` API.

## Tests

Run the documented Docker environment, then `make test-http` or `make test`.
The scanner suite covers successful and empty fields, missing delimiters of
several lengths, repeated failures, failures after earlier extraction, partial
CRLF, binary data, copies, and independent scanners. The result suite checks
every truncation point of a valid request, explicit end-of-input handling,
400/501/505 propagation, validator independence, owned request data, and parsing
after previous errors or incomplete input.

The existing known-bug suite remains enabled with unchanged expectations.
Missing Host was fixed in issue #3; issue #4 resolves all ten remaining
reproductions. See [HTTP test notes](testing_http.md) for expected and current
results.

## Baseline Docker validation for issue #3

Validated in the Ubuntu 24.04 Linux development image using
`-Wall -Wextra -Werror -std=c++98`, AddressSanitizer, and
`ASAN_OPTIONS=detect_leaks=1`:

| Check | Result |
| --- | --- |
| `make fclean`, `make re`, `make` | Pass |
| Scanner suite, including aliased delimiter/output arguments | Pass: 59 checks |
| Parsing-result suite | Pass: 437 checks |
| Existing request suite | Pass: 109 checks |
| Assertion suite | Pass: 30 checks |
| Runner self-tests | Pass: 8 tests |
| Existing URI, scheme, path, IP, keyword, and socket suites | All pass |
| Result suite rebuilt with `-fno-elide-constructors` | Pass: all 437 checks |
| `make test` | Fail: 10 of 11 C++ suites pass; known-bug suite has 13 failed assertions |

The aliasing regression was observed failing before its fix and passes afterward.
The missing-Host regression passed with 400. At the issue #3 baseline, the other
ten known-bug reproductions still failed with their original desired
expectations. Those failures are resolved in issue #4; see its
[current validation results](http_header_validation.md).
No AddressSanitizer or LeakSanitizer diagnostics appeared. No required unit
checks were blocked. Full server, epoll, and CGI integration tests were not run
because those components are outside issue #3's scope.
