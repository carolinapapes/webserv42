# Repository layout

This migration implements directory organization from GitHub issue #1. It
preserves application logic, C++98, production source selection, and
the existing compiler and AddressSanitizer flags.

All C++ filenames use lowercase snake_case. Definitions use `.cpp`, headers
use `.hpp`, and included template implementations use `.tpp`. Unit-test files
retain the `.test.cpp` suffix. Class names, function names, and other code
identifiers are unchanged.

Examples include `server_manager.cpp`/`server_manager.hpp`,
`set_up_sockets.cpp`/`set_up_sockets.hpp`, `text_colors.hpp`,
`test_assertions.hpp`, and `tree.tpp`.

## Module ownership

- `src/config/`: configuration parsing, including `lexer/`, `parser/`, and
  `symbols/`. The previous `symbol/` directory is now plural.
- `src/http/request/`, `src/http/response/`, and `src/http/uri/`: existing HTTP
  and URI implementations, with their module-specific headers.
- `src/network/`: sockets, epoll connection management, and existing
  CGI execution. CGI extraction remains a later phase.
- `src/server/`: server selection and routing, with their headers.
- `src/common/`: existing error and container utilities. `tree.hpp` and its
  template implementation `tree.tpp` stay together.
- `include/webserv/`: shared configuration types, defines, and terminal colors.
- `config/demo.conf`: the existing demo configuration, with unchanged contents.
- `examples/complete_example/http_server/`: the existing demo assets, retaining
  script permissions. `make demo` copies them to `/tmp/http_server` as before.

## Tests and preserved files

The original `tests/unit/http/uri/scheme.test.cpp` is retained. Eight unit-test
executables now run through `make test`. Objects and executables are generated
under `bin/tests/`, using
the same relative module paths. Cleanup targets remove generated output, never
test sources or fixtures.

The previously tracked `test/Scheme.test.run` executable was removed only after
the preserved unit test rebuilt and passed inside Linux.

The eleven dormant HTTP request cases now run in
`tests/unit/http/request/request.test.cpp`. Assertions compare error codes and,
for successful requests, the method, target, version, headers, and body. The
malformed Host case expects rejection, and Content-Length values match the
fixture bodies (zero for empty bodies and thirteen for the JSON example).

The commented URI, path, and IP examples from `main.cpp` now assert results in
`tests/unit/http/uri/`. IP bytes are compared with `inet_pton`. The string-helper
demo now exercises the production keyword helpers in `tests/unit/config/`.
Scanner checks replace the empty test-runner scaffold. A socket suite verifies
nonblocking listeners, descriptor cleanup, and duplicate listen configurations
using an ephemeral loopback port.

`tests/support/test_assertions.hpp` reports failed checks and returns nonzero;
the former unused output-formatting helper was removed.
`tests/integration/` is reserved for future integration tests.

The malformed configuration samples previously stored in `docs/server.conf`
and `docs/server2.conf` are preserved verbatim under `tests/fixtures/config/`.
They are parser fixtures, not runnable demo configurations.

Only the HTTP grammar from the old `mvp_parser/` is retained as
`examples/parser_prototype/HTTP.abnf`. The unused `RecursiveDescent.cpp` and
`RecursiveDescent.hpp` prototype files were removed at the user's request.
The unused legacy `include/webserv/webserv.hpp`, unreferenced KeyPoint, EmptyLine,
Block and HTTP parser stubs, and duplicate `str_match_utils.*` implementation
were removed after checking their references. The duplicate helper's examples
are covered by tests of `a_key_word.cpp`.

The dormant event logger, uncalled debug printers and query validator, unused
pipe-mapping helper, empty `control_data.cpp`, and commented implementation
experiments were also removed. The query validator remains inactive as before;
this cleanup does not enable new validation rules. The production build now
contains 73 source files, excluding the former logger and empty translation unit.

## 42 file headers

All 182 supported code, script, build, and example files have a 42 banner.
Existing credits are preserved. Missing banners use the repository's
`capapes <capapes@student.42.fr>` identity and the appropriate comment syntax.
Shebangs remain first; PHP banners follow the opening PHP tag, and HTML banners
follow the doctype. ABNF references use semicolon comments. Configuration files
are unchanged because this configuration parser does not support comment lines.

## Linux validation

Build `Dockerfile.dev` and run compilation, tests, and the server inside the
`webserv-dev` image, as described in the README and `AGENTS.md`. CI uses the
same Dockerfile.

Before the moves, the unchanged revision passed `make` and `make test` inside
this Linux image using GCC 13.3.0 on arm64. The single discovered URI unit test
was rebuilt from source and passed.

After the migration, the same Linux environment passed:

- `make -j4` and `make test`.
- `make -q` and `make -q build_test`, confirming incremental builds are current.
- `make fclean`, with checks that all test sources and fixtures remain intact.
- `make re` and a subsequent `make test`.
- `zsh -n run_tests.sh`.

The directory and filename migration's C++ comparison permitted include-directive
and filename banner changes.
The production source selection and order, compiler flags, examples, fixture
contents, and executable script permissions were also checked against the
original revision. Object filenames follow the renamed source files.

Those checks passed again after normalizing 155 C++ filenames. At that stage,
all 170 C++ files followed the naming convention, and the 75 production source
files retained their original selection and link order. The original unit test is rebuilt as
`bin/tests/http/uri/scheme.test.run`.

After the dead-code cleanup and header additions, Linux validation passed the
build, all eight unit executables, incremental builds, `make fclean`, `make re`,
and the tests again after rebuilding. The suites perform 117 checks in total,
including the original scheme check. A deliberately failed assertion was also
verified to make the shell runner return a failure status. The development image
build, Python syntax checks, and shell syntax check passed.

A token comparison of retained production code permits only the explicitly
removed unused helpers, declarations, fields, and no-op branch, together with
include and whitespace changes. All remaining production `.cpp` files are built;
no commented `main` functions remain in production sources.

### Existing demo startup limitation

The original `make demo` and the migrated `make demo` and `make run` all reach
socket setup and exit with `bind: Address family not supported by protocol` in
this image. The demo's `localhost` listen addresses and socket implementation
were preserved; repairing address-family handling is outside this issue.

For the final runtime comparison, a temporary configuration inside the container
replaced only `listen localhost:` with `listen 127.0.0.1:`. No repository
configuration was changed. The original binary and the cleaned binary used the
same current example assets, including their added comment headers, and returned
identical response bytes for:

| Request | Existing status | Response bytes |
| --- | --- | --- |
| `GET /` | `200 OK` | 3143 |
| `GET /redirection` | `302 Found` | 159 |
| `GET /teapot` | `418 Unknown` | 239 |
| `GET /bonus.py` | `200 OK` | 3155 |

The last request exercised the preserved Python CGI path. These smoke checks do
not constitute comprehensive HTTP, epoll, CGI, or resource-leak testing.
