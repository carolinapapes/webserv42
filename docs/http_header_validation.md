# HTTP header storage and validation

Issue [#4](https://github.com/carolinapapes/webserv42/issues/4) corrects header
storage and HTTP validation on top of issue #3's explicit parsing results.
Application code remains C++98 with no new dependencies.

## Header API

Headers stores an ordered sequence of name/value pairs, preserving each field's
original spelling, value, and arrival order. All name lookups compare ASCII
letters without case sensitivity.

| Operation | Behavior |
| --- | --- |
| `add(name, value)` | Appends an occurrence without overwriting earlier fields |
| `getAll(name)` | Returns an owned vector of all values in arrival order |
| `get(name)` | Returns the first value, or an empty string if absent |
| `has(name)` | Distinguishes absence from a present field with an empty value |
| `remove(name)` | Removes every occurrence, ignoring name case |
| `operator[](name)` | Returns the first value for explicit editing; inserts an empty value if absent |
| Iteration | Visits every original name/value pair in arrival order |

The existing map constructor, copy operations, iterators, and single-value access
remain available to callers. Equality compares names without case sensitivity,
preserves the order of repeated values, and ignores ordering between different
names. Values are case-sensitive.

```cpp
#include <headers.hpp>

Headers headers;
headers.add("X-Trace", "first");
headers.add("x-trace", "second");
std::vector<std::string> values = headers.getAll("X-TRACE");
// values contains "first", "second"; headers.get("x-trace") is "first"
```

The ordered container uses `std::vector`; adding or removing fields can
invalidate iterators and references returned by `get` or `operator[]`.
`getAll` returns an independent snapshot. Parsed fields use `add` so duplicate
validation always sees every occurrence.

## Field syntax and Host

Names accept the HTTP token character set. Generic values may be empty and may
contain HTAB, printable bytes, and opaque high bytes. NUL, DEL, and other control
bytes are rejected. Only SP and HTAB are trimmed as optional whitespace;
vertical tab and form feed are not silently removed. Obsolete folded lines and
whitespace before a field's colon produce 400.

The server requires one nonempty Host field. Duplicate names are detected across
case variants. Host syntax excludes user information, paths, queries, fragments,
and whitespace. Bracketed IPv6 and IPvFuture literals, percent-encoded reg-names,
and optional decimal ports are checked before use. Ports must be nonempty when
the colon is present and fit 0 through 65535. This avoids passing malformed
authority input to the legacy URI constructors.

The existing field-name/value bounds of 256 and 8192 bytes remain. Aggregate
request and body limits belong to the incremental framing work in issue #5.

## Framing-header validation

`parseContentLength(text, length)` validates decimal digits and returns the
numeric result through a `size_t` reference. It detects overflow before
multiplication, resets the output to zero on failure, and enforces the project's
existing INT_MAX bound. INT_MAX is an implementation bound, not an HTTP limit.
`isValidContentLength` uses the same implementation. The broken boolean-valued
`string_to_long` helper is removed.

Repeated Content-Length fields are retained and accepted when all numeric
values match, including different leading-zero spellings. Conflicts and malformed
values produce 400. Comma-separated Content-Length values are rejected, including
identical lists; rejection is an allowed policy for these values.

Transfer-Encoding combined with Content-Length produces 400 regardless of
header-name case or ordering. Transfer-coding lists are parsed across repeated
fields, including quoted parameters and ignored empty list elements. A request
must end in exactly one chunked coding. Duplicate or non-final chunked coding,
chunked parameters, missing final chunked coding, and malformed lists produce
400. Otherwise valid unsupported coding chains produce 501.

This issue validates framing headers. It does not decode chunked data, enforce
the received body length, split pipelined requests, or determine when fragmented
bodies are complete. The current parser still returns the available body bytes;
issue #5 supplies incremental framing and decoding.

## Request-line validation

Method syntax and server support are separate checks. Malformed tokens produce
400; syntactically valid unsupported methods produce 501. Existing server support
remains GET, POST, PUT, and DELETE. Only HTTP/1.1 is supported: other syntactically
valid versions produce 505, while malformed versions produce 400.

Origin-form and HTTP(S) absolute-form targets are validated. CONNECT requires
authority-form with a port, and only OPTIONS accepts asterisk-form. CONNECT and
OPTIONS remain unsupported methods and produce 501 after valid syntax checks.
Relative targets, fragments, invalid raw URI characters, malformed authorities,
and malformed percent escapes in either paths or queries produce 400.

The existing URI parser continues to validate path/query components. It receives
a fixed validated authority for that check, keeping its internal implementation
unchanged. Original request-target text and all header fields remain available
on the Request. Socket handling, CGI, and routing are unchanged.

## Parse-error table

`request_parse_error.cpp` defines the single table of parse-error IDs, HTTP status
codes, and diagnostic messages. Schema definitions and validators refer to
`RequestParseError::Id` values rather than embedding message strings or codes at
each failure site. The exception constructor resolves the ID through that table.
Existing error messages and status codes are preserved. An unknown ID has a 500
fallback diagnostic. Compilation checks the table's size, and tests verify that
every declared error ID has a message and defined HTTP status.

## References

- [RFC 9110 §5](https://www.rfc-editor.org/rfc/rfc9110.html#section-5): field syntax, case, values, and list parsing.
- [RFC 9110 §8.6](https://www.rfc-editor.org/rfc/rfc9110.html#section-8.6): Content-Length syntax and duplicate-list policy.
- [RFC 9112 §3](https://www.rfc-editor.org/rfc/rfc9112.html#section-3): request-line and target forms.
- [RFC 9112 §6](https://www.rfc-editor.org/rfc/rfc9112.html#section-6): framing-header precedence and transfer codings.

## Tests

Use the documented Linux Docker environment, then run `make test` or the focused
`make test-http` target. `make test-http-regressions` retains all eleven original
bug reproductions and their 32 checks, which now pass with unchanged expectations.
The new header-storage and field-validation suites cover repeated fields,
case-insensitive access, empty values, control bytes, Host syntax, numeric bounds,
coding lists, method/version checks, and all request-target forms.

## Docker validation for issue #4

Validated in the Ubuntu 24.04 Linux development image with
`-Wall -Wextra -Werror -std=c++98`, AddressSanitizer, and leak detection enabled.

| Check | Result |
| --- | --- |
| `make fclean`, `make re`, `make` | Pass |
| `make -j4 test` | Pass: all 13 C++ suites and all 8 Python runner tests |
| Header storage | Pass: 33 checks |
| HTTP field validation | Pass: 466 checks |
| Parsing results and error table | Pass: 501 checks, including coverage of all 29 error IDs |
| `make test-http-regressions` | Pass: all 32 original checks |
| Live-server smoke test | Fail: connection reset on the first static GET; AddressSanitizer reports a heap-use-after-free in `ServerManager::~ServerManager` |
| Remaining live-server cases | Not run after the first failure, including routing, CGI, and malformed-request responses |

No AddressSanitizer or LeakSanitizer diagnostics appeared during the unit tests.
The live-server failure was also reproduced by building and running the
unchanged merged baseline, commit `70c476b6631543336e5bfb99943c77db06fe5cb1`,
in a separate Docker container. Its stack shows the same ServerManager cleanup
failure through `EpollConnectionManager`. Server ownership and event-loop changes
remain outside this issue, so successful end-to-end responses are not established
by these results.
