# Linux/Unix Shell & Systems Programming Lab — Q31–Q64

Solutions for the second half of the OS Lab practical exam set, covering IPC
(pipes, FIFOs, message queues, shared memory, semaphores), sockets, process
scheduling, timers, resource limits, and signal handling in C on
Linux/Ubuntu (developed and tested under WSL).

Each question lives in its own numbered folder (`31/`, `32/`, … `64/`),
matching how the programs were written and tested during preparation.

## Contents

| # | Topic | Files |
|---|---|---|
| 31 | Anonymous pipe, no `fork()` | `31.c` |
| 32 | Pipe + `fork()`, parent writes / child reads | `32.c` |
| 33 | Bidirectional IPC via two pipes + `fork()` | `33.c` |
| 34 | `ls -l \| wc` via `fork()` + redirection (3 variants: `close()`+`dup()`, `dup2()`, `fcntl(F_DUPFD)`) | `34a.c`, `34b.c`, `34c.c` |
| 35 | 3-stage pipeline `ls -l \| grep ^d \| wc -l` via two pipes + 3 children | `35.c` |
| 36 | Creating a FIFO five ways (`mknod`/`mkfifo` shell commands, `strace` comparison, `mknod()`/`mkfifo()` in C) | `36a.c`…`36e.c` |
| 37 | FIFO IPC between **unrelated** processes | `sender.c`, `receiver.c` |
| 38 | Bidirectional IPC via two named FIFOs | `program1.c`, `program2.c` |
| 39 | `select()` on a non-blocking FIFO with timeout | `39.c` |
| 40 | `getrlimit(RLIMIT_NOFILE)` + `fcntl(F_GETPIPE_SZ)` | `40.c` |
| 41 | `ftok()` + `msgget()` — create a message queue | `41.c` |
| 42 | `msgctl(IPC_STAT)` — dump message queue status | `42.c` |
| 43 | `msgsnd()` — send a message | `43.c` |
| 44 | `msgrcv()` — blocking (flag `0`) vs non-blocking (`IPC_NOWAIT`) receive | `44.c` (or `44a.c`/`44b.c`) |
| 45 | `msgctl(IPC_STAT/IPC_SET)` — read and change queue permissions | `45.c` |
| 46 | `msgctl(IPC_RMID)` — remove a message queue | `46.c` |
| 47 | Shared memory: create, attach read-write, write, detach, re-attach read-only, remove | `47.c` |
| 48 | Binary vs counting semaphores (`semget`/`semctl(SETVAL/GETVAL)`) | `48a.c`, `48b.c` |
| 49 | Semaphore-protected ticket counter, producer/consumer shared memory, counting semaphore with 4 processes, manual removal | `common.c`, `init_ticket.c`, `ticket.c`, `sharedsem.c`, `counting.c`, `rem_sem.c` |
| 50 | Deliberate deadlock with two semaphores (circular wait) | `50.c` |
| 51 | TCP client/server sockets | `server.c`, `client.c` |
| 52 | Concurrent TCP server: one process per client (`fork`) vs one thread per client (`pthread`) | `server_fork.c`, `server_thread.c` |
| 53 | Interval timers: `ITIMER_REAL`, `ITIMER_VIRTUAL`, `ITIMER_PROF` | `real.c`, `virtual.c`, `prof.c` |
| 54 | `getrlimit()` — print CPU/file-size/open-files/processes/stack/core/virtual-memory limits | `54.c` |
| 55 | `setrlimit()` — lower `RLIMIT_NOFILE`'s soft limit | `55.c` |
| 56 | Measuring `getppid()` syscall overhead with the x86 `RDTSC` instruction | `56.c` |
| 57 | `sysconf()` + `sysinfo()` — system configuration limits and memory stats | `57.c` |
| 58 | Basic `pthread_create()` / `pthread_join()` | `58.c` |
| 59 | `signal()`-based handlers: `SIGSEGV`, `SIGINT`, `SIGFPE`, `SIGALRM` (via `alarm()` and `setitimer()`), `SIGVTALRM`, `SIGPROF` | `sigsegv.c`, `sigint.c`, `sigfpe.c`, `sigalr_alarm.c`, `sigalr_timer.c`, `sigvtalrm.c`, `sigprof.c` |
| 60 | `signal()` — `SIG_IGN` then `SIG_DFL` for `SIGINT` | `60.c` |
| 61 | Same handlers as Q59 (`SIGSEGV`, `SIGINT`, `SIGFPE`), rewritten with `sigaction()` | `sigsegv.c`, `sigint.c`, `sigfpe.c` |
| 62 | `sigaction()` — `SIG_IGN` then `SIG_DFL` for `SIGINT` | `62.c` |
| 63 | Orphan process: child sends `SIGKILL` to its own parent | `orphan.c` |
| 64 | `SIGSTOP` cannot be caught/blocked/ignored — sender/receiver demo | `sender.c`, `receiver.c` |

## Build & run

Every program is a standalone C file, compiled individually with `gcc`:

```bash
gcc <file>.c -o <name>
./<name>
```

A few folders need extra flags or a specific run order:

- **Q52 (`server_thread.c`)** — needs the pthread library:
  ```bash
  gcc server_thread.c -o server_thread -lpthread
  ```
- **Q58** — also needs `-lpthread`.
- **Q49** — run `init_ticket` once before launching multiple `ticket` processes concurrently; run `sharedsem` and `counting` independently; `rem_sem` is a fallback cleanup tool only needed if `counting.c` didn't reach its own cleanup.
- **Q51/Q52** — run the server first in one terminal, then the client in another. For same-machine testing, point the client at `127.0.0.1`.
- **Q64** — run `receiver` in one terminal (note it, grab its PID), then `sender <PID>` from another.

## Common pitfalls hit (and fixed) during testing

- **`ftok(path, id)` depends on the inode of `path`.** Using `ftok(".", ...)`
  means the resulting key changes depending on which directory you run the
  program from. Every message-queue/semaphore program in Q41–50 was fixed to
  use a path with a stable location (e.g. `ftok("/tmp", ...)`) instead of
  `"."`, so the same key is produced regardless of the current working
  directory.
- **`write(fd, buf, count)` needs an exact byte count** — it does not stop at
  a null terminator the way `printf("%s", ...)` does. A few handlers in
  Q59/Q61 originally passed the wrong literal byte count for a string,
  producing garbled output; always count the characters (or use
  `sizeof(str) - 1`) instead of guessing.
- **Only async-signal-safe functions belong inside a signal handler**
  (`write()`, `_exit()` — not `printf()`/`exit()`), since the handler can
  interrupt the program at an arbitrary point, including mid-`stdio`
  operation.
- **`ITIMER_VIRTUAL`/`ITIMER_PROF` only tick while the process is actually on
  the CPU** — a busy-loop is required to observe them; `pause()`/`sleep()`
  never lets these two fire, unlike `ITIMER_REAL`.
- **`SIGKILL` and `SIGSTOP` can never be caught, blocked, or ignored** —
  `sigaction()` on either always fails with `EINVAL`, by design.
- **IPC resources (message queues, semaphores, shared memory) are
  kernel-persistent**, not process-lifetime-bound — always clean them up
  explicitly (`IPC_RMID`) or with `ipcrm`, otherwise they linger until
  reboot. `ipcs -q` / `-m` / `-s` lists what's currently alive;
  `ipcrm -s/-m/-q <id>` (the numeric ID column, not the hex key) removes
  it.
- **A server left running in the background** holds its port open —
  `bind: Address already in use` usually means an old server process is
  still alive; find it with `sudo lsof -i :<port>` and `kill` it, rather
  than assuming `SO_REUSEADDR` alone will fix it (that only helps with a
  lingering `TIME_WAIT` socket, not a genuinely live listener).

## Environment

Developed and tested on Ubuntu via WSL. A few commands (`who`, `users`,
network-facing tools) can behave differently under WSL than on bare-metal
Linux — noted individually where relevant.