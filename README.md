# Raccon

Raccon is a small, event-driven in-memory key-value server implemented in C. It uses epoll for scalable I/O, a custom RESP-like parser, and an in-memory dictionary backing store.

## Features
- Non-blocking TCP server using epoll
- RESP-like protocol parsing (bulk/simple strings, integers, doubles)
- Commands: `PING`, `SET`, `GET`, `DEL`
- Simple in-memory dictionary with basic object/value support

## Project layout
- `src/` — implementation (.c)
- `include/` — public headers (.h)
- `debug_build/` — out-of-tree build output (created by CMake)
- `main.c` — program entry (accepts address and port)

Important headers:
- `include/evl.h` — event loop API
- `include/socket.h` — socket helpers
- `include/parser.h`, `include/parser_ctx.h` — protocol parsing
- `include/command.h`, `include/execute.h` — command model and execution

## Build
Requires CMake and a C compiler supporting C23.

Recommended out-of-source build (Debug):

```bash
cmake -S . -B debug_build -DCMAKE_BUILD_TYPE=Debug
cmake --build debug_build
```

There is also a VS Code task named `build` that runs `cmake --build debug_build`.

## Run
Run the built binary. The program accepts optional `address` and `port` arguments (defaults: `127.0.0.1 8080`):

```bash
./debug_build/evl_test [address] [port]
# example: bind to all interfaces on port 6379
./debug_build/evl_test 0.0.0.0 6379
```

You can interact using `nc`/`telnet` or a RESP-capable client. Example using `nc` (simple PING):

```bash
printf "*1\r\n$4\r\nPING\r\n" | nc 127.0.0.1 8080
```

## Protocol notes
Raccon implements a subset of a RESP-like protocol:
- Simple strings: `+OK\r\n`
- Bulk strings: `$<len>\r\n<payload>\r\n`
- Integers: `:<number>\r\n`
- Doubles : `,<number>\r\n`
- Boolean : `#<bool>\r\n`
- Multi-bulk arrays to compose commands: `*<n>\r\n` followed by n elements

Parser and command validation live in `src/parser.c` and `src/execute.c`.

