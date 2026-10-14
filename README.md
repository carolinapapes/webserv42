# webserv42

A 42 School project implementing a HTTP web server in C++98.

The goal of this project is to understand how web servers work internally: sockets, HTTP request parsing, configuration files, routing rules, file descriptors, non-blocking I/O, and server response handling.

---

## What this project shows

* HTTP server implementation in C++98
* Socket setup and server listening flow
* Event-driven connection handling with epoll
* HTTP request parsing
* HTTP response generation
* Configuration file parsing
* URI parsing and path handling
* Multiple server/location configuration structure
* Error handling with custom exceptions
* Makefile-based C++ build workflow
* Systems programming concepts applied to web infrastructure

---

## Tech stack

* C++98
* Unix sockets
* epoll
* HTTP
* Makefile

---

## How it works

The server reads a configuration file, parses the server/location rules, sets up the required sockets, and starts handling incoming HTTP connections.

Basic flow:

```text
Configuration file
        ↓
Lexer / parser
        ↓
Server configuration
        ↓
Socket setup
        ↓
Connection manager
        ↓
Request parsing
        ↓
Response handling
```

---

## Usage

Clone the repository:

```bash
git clone git@github.com:TahereTaha/webserv42.git
cd webserv42
```

Build the Linux development image (also used by CI):

```bash
docker build -f Dockerfile.dev -t webserv-dev .
```

Start a development container from the repository root:

```bash
docker run --rm -it -v "$(pwd):/app" -w /app -p 8080:8080 webserv-dev
```

Run the following commands inside the Linux container. The server uses Linux-specific APIs, including epoll.

Build the project:

```bash
make
```

Prepare the demo files under `/tmp/http_server` and run the server:

```bash
make demo
```

`make run` starts the same demo. Its configuration is `config/demo.conf`;
its existing listen addresses and runtime paths are preserved. After preparing
the demo files, you can also invoke `./webserv config/demo.conf` directly.

The unchanged demo currently reports `bind: Address family not supported by
protocol` in the Linux development image with its `localhost` listen addresses.
This also occurs before the migration. See the layout notes for validation
using a temporary IPv4 configuration.

Run the test target:

```bash
make test
```

Test sources and fixtures live under `tests/`. Generated test objects and
executables live under `bin/tests/`. Eight suites cover the original scheme test,
URI references, paths, IP bytes, HTTP requests, request scanning, keyword helpers,
and listening sockets. The old print-only examples and disabled request checks
now run as assertions and return a failure status when a check fails.

Clean object files:

```bash
make clean
```

Remove object files and the binary:

```bash
make fclean
```

Rebuild from scratch:

```bash
make re
```

---

## Project structure

```text
.
├── .github/workflows/     # CI using the Linux development image
├── config/               # Demo server configuration
├── docs/                 # Reference documentation and development notes
├── examples/
│   ├── complete_example/ # Demo assets and CGI scripts
│   └── parser_prototype/ # HTTP ABNF grammar reference
├── include/webserv/      # Shared headers and configuration types
├── src/
│   ├── main.cpp
│   ├── args.cpp          # Argument handling, with its header alongside
│   ├── common/
│   │   ├── containers/
│   │   └── errors/
│   ├── config/           # Configuration parser, lexer, and symbols
│   ├── http/
│   │   ├── request/
│   │   ├── response/
│   │   └── uri/
│   ├── network/          # Sockets, epoll, and existing CGI execution
│   └── server/           # Server selection and routing
├── tests/
│   ├── unit/
│   ├── integration/      # Reserved for future integration tests
│   ├── fixtures/         # Preserved malformed configuration examples
│   └── support/
├── Dockerfile.dev
├── Makefile
└── run_tests.sh
```

Module-specific headers stay alongside their implementations. Shared headers
are included as `<webserv/...>`. C++ filenames use lowercase snake_case, with
`.cpp` sources, `.hpp` headers, and `.tpp` template implementations. Class and
function names are unchanged. See [repository layout notes](docs/repository_layout.md)
for preserved legacy files and validation details.

---

## Main concepts practiced

### HTTP server behavior

The project explores how a server receives requests, parses them, and returns valid HTTP responses.

### Configuration parsing

The server is configured through `.conf` files with server and location blocks, similar in spirit to common web server configuration formats.

### Non-blocking connections

Connection handling is organized around an event-driven model using epoll, allowing the server to manage multiple connections efficiently.

### URI and request parsing

The project includes custom parsing logic for HTTP requests and URI components, which helps build a deeper understanding of how web requests are structured.

### C++98 architecture

Because the project follows C++98 constraints, the code relies on manual design decisions, custom structures, clear ownership, and careful error handling.

---

## Why this project matters

`webserv42` is a systems programming project that connects low-level programming with web development.

It helps build practical understanding of concepts that are useful in backend and full-stack development:

* How HTTP works under the hood
* How servers manage sockets and clients
* How configuration-driven applications are structured
* How requests are parsed and transformed into responses
* How infrastructure tools like Nginx are conceptually built
* How to design larger C++ projects with multiple modules

---

## Repository topics

Suggested GitHub topics:

```text
cpp
cpp98
http-server
web-server
sockets
epoll
systems-programming
network-programming
http
makefile
42-school
42-project
```
