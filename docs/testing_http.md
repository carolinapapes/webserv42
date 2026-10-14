# HTTP request parser tests

Issue [#2](https://github.com/carolinapapes/webserv42/issues/2) establishes tests
before changing the parser. The old `Test` and `UnitTestRunner` files were moved
and consolidated during issue #1; the current tests live in
`tests/unit/http/request/`. Issue #3 builds on these tests with safe extraction
and [explicit parsing results](http_parser_results.md).

## Running in Linux

From the repository root:

```sh
docker build -f Dockerfile.dev -t webserv-dev .
docker run --rm -it -v "$PWD:/app" -w /app webserv-dev
```

Inside the container:

```sh
make
make test-framework
make test-http
make test-http-regressions
make test
```

`make test-framework` verifies matching results, incorrect values for every
request field, missing and extra headers, process exit codes, an empty or missing
suite directory, paths containing spaces, signal termination, and continued
execution after a failed suite. It uses only Python's standard library, zsh, and
the compiled C++ assertion test. It should pass.

`make test-http` builds and runs all five request suites, including known bugs.
`make test-http-regressions` runs only the known-bug suite. `make test` runs all
eleven C++ suites and the runner self-tests. These three targets currently return
nonzero because known parser bugs are asserted as failures. A test executable
returns 1 on a failed assertion; the shell runner returns 1 if any suite fails;
GNU Make returns 2 when a recipe fails. Do not interpret parser error messages
alone as a test failure: malformed-input tests expect the parser to reject them.

To run the passing request/scanner suites after building:

```sh
./bin/tests/http/request/request.test.run
./bin/tests/http/request/req_scanner.test.run
./bin/tests/http/request/parse_result.test.run
```

## Coverage and fixture expectations

`request.test.cpp` retains all eleven original scenarios and extends coverage
to nine valid and twenty-seven invalid requests. Successful requests compare the
error code (0 means success), method, target, version, exact header map, and body.
Rejected requests compare their expected error code; partially populated parser
state after rejection is not asserted. Unexpected exceptions fail the test.

Valid fixtures independently check the header terminator, body bytes, and
declared Content-Length against the expected body's byte count. The JSON fixture
`{"key":"value"}` is 15 bytes; its old value of 13 was corrected. A malformed Host
fixture remains intentionally invalid. A binary POST checks preservation of NUL
and CRLF bytes. Existing GET, JSON POST, PUT, and query scenarios are retained;
DELETE, optional header whitespace, embedded colons, and encoded paths are added.

Invalid cases cover missing line components, an unsupported method/version,
missing headers/terminator/Host value, invalid header names/values, empty,
negative, signed, nonnumeric, fractional, and conflicting-list Content-Length
values, and invalid/truncated percent escapes in paths. Three direct validator
checks cover zero, a positive length, and its existing Linux INT_MAX boundary.

The C++ tests use checks that remain active with `NDEBUG`. The assertion suite
deliberately changes each output field independently, while the Python tests
launch those deliberate failures as subprocesses and require exit code 1.

## Confirmed parser failures

`known_bugs.test.cpp` contains desired assertions, without skipped tests or
expected-failure conversion. Issue #3 fixes the missing-Host error-propagation
case. The other ten reproductions still produce thirteen failing assertions:

| Reproduction | Expected | Current result |
| --- | --- | --- |
| No Host, with User-Agent present | 400 | 400; fixed in issue #3 |
| Two Host fields | 400 | 0; last field overwrites first |
| Content-Length fields of 3 and 4 | 400 | 0; last value wins |
| `cOnTeNt-LeNgTh: nope` | 400 | 0; validation bypassed |
| `/search?q=%GG` | 400 | 0; query escape not validated |
| `GET / HTTP/1.1 extra` | 400 | 505 |
| Lowercase `host` | Successful parse with header retained | 400; name lookup still case-sensitive |
| Empty optional `X-Empty:` | Successful parse with header retained | 400 |
| Valid field name `X!Trace` | Successful parse with header retained | 400 |
| Numeric validator: `2147483648` | false under its existing INT_MAX bound | true |
| Numeric validator: thirty nines | false on conversion overflow | true |

Missing/duplicate Host expectations follow
[RFC 9112 §3.2](https://www.rfc-editor.org/rfc/rfc9112.html#section-3.2);
conflicting lengths follow
[RFC 9112 §6.3](https://www.rfc-editor.org/rfc/rfc9112.html#section-6.3).
Field-name and field-value cases follow
[RFC 9110 §5](https://www.rfc-editor.org/rfc/rfc9110.html#section-5).
The extra-token case follows the request-line grammar in
[RFC 9112 §3](https://www.rfc-editor.org/rfc/rfc9112.html#section-3), and query
escapes follow the URI grammar used by HTTP request targets.

The two numeric tests document the bound already expressed by the project's
validator; INT_MAX is not an HTTP protocol limit. They call the validator directly
to avoid representing a huge Content-Length as an otherwise valid request with
an inconsistent body.

These remaining failures belong to later parser work. Issues #2 and #3 do not
implement HTTP body framing or change epoll, CGI, or routing. Full server/CGI
integration and an exhaustive HTTP conformance suite are not part of these unit
tests.

## Baseline Docker validation for issue #2

Validated in the Ubuntu 24.04 development image with
`-Wall -Wextra -Werror -std=c++98`, AddressSanitizer, and
`ASAN_OPTIONS=detect_leaks=1`:

| Check | Result |
| --- | --- |
| `make fclean`, `make re`, `make` | Pass |
| Request suite | Pass: 109 checks |
| Scanner suite | Pass: 10 checks |
| Assertion suite | Pass: 30 checks |
| Runner self-tests | Pass: 8 tests, including eight deliberate field failures |
| Existing URI, scheme, path, IP, keyword, socket suites | All pass |
| `make test` | Fail: 9 of 10 C++ suites pass; known bugs have 13 failed assertions |
| `make test-http-regressions` | Fail: the same 11 known-bug reproductions |

There were no AddressSanitizer or LeakSanitizer diagnostics. No required unit
checks were blocked. Full server, epoll, and CGI integration tests were not run
for this test-framework issue. These counts record the issue #2 baseline; see
the [issue #3 API notes](http_parser_results.md) for its validation results.
CI runs `make test` and will report the remaining parser regressions as failures
until later parser work resolves them.
