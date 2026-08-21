# OS Lab 3 — Fork and Exec

Processes and `fork()` in C: fork semantics, `wait()`/`waitpid()` and exit-status
macros, the `exec` family, pipes and pipelines, and zombie/orphan processes.

## Layout

```
os-lab/
├── CMakeLists.txt        # CMake build (all targets)
├── Makefile              # Make build (all targets -> bin/)
├── README.md
├── src/                  # walkthrough demos
│   ├── fork_demo.c       # fork + waitpid + WIFEXITED/WEXITSTATUS
│   ├── exec_demo.c       # execlp("ls","-l")
│   ├── pipe_demo.c       # parent writes 5 lines -> child runs `wc -l`
│   ├── pipeline_demo.c   # ls | grep <pattern> (2 forks, 1 pipe)
│   └── zombie_demo.c     # child exits, parent sleeps -> zombie
└── challenges/           # 10 coding challenges
    ├── ch1_single_fork.c
    ├── ch2_multi_child.c
    ├── ch3_exec_ls.c
    ├── worker.c          # helper binary for ch4
    ├── ch4_exec_worker.c
    ├── ch5_exec_examples.c
    ├── ch6_pipe_sum.c
    ├── ch7_pipeline.c
    ├── ch8_wait_nonblock.c
    ├── ch9_zombie.c
    └── ch10_pool.c
```

## Build

**Make** (binaries land in `bin/`):
```bash
make
./bin/fork_demo
```

**CMake** (binaries land in `build/`):
```bash
mkdir -p build && cd build
cmake ..
cmake --build .
./fork_demo
```

`make clean` removes `bin/` and `build/`.

## Demos (src/)

| Program          | What it shows                                    | Expected |
|------------------|--------------------------------------------------|----------|
| `fork_demo`      | parent/child split, reap child                   | child exits with status 42 |
| `exec_demo`      | replace child image with `ls -l`                 | directory listing + parent message |
| `pipe_demo`      | parent → pipe → child `wc -l`                     | prints `5` |
| `pipeline_demo`  | manual `ls \| grep <pattern>` (default `\.c$`)    | matching filenames |
| `zombie_demo`    | child zombie for 10s until parent `wait()`s       | see Z in `ps -l` |

## Challenges — status & verification

All 10 implemented and verified.

| # | File | Acceptance | Verify |
|---|------|------------|--------|
| 1 | `ch1_single_fork.c` | parent reports child status 7 | `./bin/ch1_single_fork` |
| 2 | `ch2_multi_child.c` | N children, each `WEXITSTATUS` reported | `./bin/ch2_multi_child 5` |
| 3 | `ch3_exec_ls.c` | child runs `ls -la`, parent prints after | `./bin/ch3_exec_ls` |
| 4 | `ch4_exec_worker.c` (+`worker.c`) | worker prints args and `MYVAR=hello` | `./bin/ch4_exec_worker` |
| 5 | `ch5_exec_examples.c` | both children print `one two` | `./bin/ch5_exec_examples` |
| 6 | `ch6_pipe_sum.c` | child prints `Sum = 55` | `./bin/ch6_pipe_sum` |
| 7 | `ch7_pipeline.c` | matches `ls \| grep <pattern>` | `./bin/ch7_pipeline "\.c$"` |
| 8 | `ch8_wait_nonblock.c` | `WNOHANG` poll, finishes ascending | `./bin/ch8_wait_nonblock` |
| 9 | `ch9_zombie.c` | child shows Z in `ps -l` during sleep | `./bin/ch9_zombie` then `ps -l` |
| 10 | `ch10_pool.c` | bounded concurrency, ≤ M workers | `./bin/ch10_pool 3 f1 f2 f3 f4 f5` |

For ch4, run from the directory containing the `worker` binary (or `bin/`),
since it execs `./worker`.

## Notes / known issues

- Children after `fork()` use `_exit()` and `fflush(stdout)` before exiting to
  avoid double-flushing the inherited stdio buffer (relevant in ch6, ch10 where
  the parent also prints).
- `pipeline_demo`/`ch7` only show output when matching files exist in the
  current working directory (run from `challenges/` to match `.c` files).
- Every `fork()`/`exec*()` return value is checked with `perror()` on failure.
- Zombie observation (ch9, zombie_demo) requires a second terminal running
  `ps -l` during the parent's sleep window.

## Completed

All demos (5/5) and all challenges (10/10) build warning-free with
`-Wall -Wextra` and run with the expected output.
