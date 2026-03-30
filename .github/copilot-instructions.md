# xv6-riscv Copilot Instructions

## Response Language

- Default to Chinese in responses.
- If the user explicitly asks for another language, follow the user's request.

## General Assistant Behavior

From now on, act as my expert assistant with access to all your reasoning and knowledge. Always provide:

- A clear, direct answer to my request.
- A step-by-step explanation of how you got there.
- Alternative perspectives or solutions I might not have thought of.
- A practical summary or action plan I can apply immediately.

Never give vague answers. If the question is broad, break it into parts. If I ask for help, act like a professional in that domain (teacher, coach, engineer, doctor, etc.). Push your reasoning to 100% of your capacity.

## Build, test, and debug commands

- Build kernel and user programs: `make`
- Build and run in QEMU (normal workflow): `make qemu`
- Run with GDB stub enabled: `make qemu-gdb`
- Clean generated artifacts: `make clean`
- Generate tags: `make tags`

Testing is executed inside the xv6 shell after boot (`make qemu`):

- Full user-space regression tests: `usertests`
- Fast/quiet test mode: `usertests -q`
- Run a single test case: `usertests <testname>` (example: `usertests copyin`)

There is no dedicated lint target in this repository.

## High-level architecture

- Boot and kernel bring-up:
  - `kernel/start.c` enters supervisor mode and jumps to `kernel/main.c`.
  - `kernel/main.c` initializes memory, traps/interrupts, process table, file system, and disk, then enters `scheduler()`.

- User/kernel boundary and syscall flow:
  - User syscall stubs are generated from `user/usys.pl` into `user/usys.S`.
  - Stub sets `a7=SYS_*` and executes `ecall`.
  - Trap handling is in `kernel/trap.c` (`usertrap`/`kerneltrap`).
  - Syscall dispatch is in `kernel/syscall.c`, with implementations split across `kernel/sysproc.c` and `kernel/sysfile.c`.

- Process lifecycle:
  - Process table/scheduler and `userinit()` are in `kernel/proc.c`.
  - First user code comes from `user/initcode.S`, then runs `/init`.
  - `user/init.c` keeps spawning `sh`; shell exit causes restart.

- File system layering:
  - `kernel/fs.c` documents/implements layered FS design (blocks, log, inodes/files, directories, names).
  - `kernel/log.c` provides transactional logging.
  - FS syscalls in `kernel/sysfile.c` wrap mutating operations with `begin_op()`/`end_op()`.

- Build/image pipeline:
  - `Makefile` defines kernel objects and `UPROGS`.
  - `mkfs/mkfs` builds `fs.img` from `README` plus compiled `user/_*` binaries.

## Key repository conventions

- User program packaging:
  - Add new user binaries to `UPROGS` as `$U/_name` in `Makefile`.
  - In xv6 shell, run them as `name` (without underscore).

- Syscall extension touchpoints (keep all in sync):
  - Add syscall number in `kernel/syscall.h`.
  - Add kernel handler declaration and `syscalls[]` entry in `kernel/syscall.c`.
  - Implement handler in `kernel/sysproc.c` or `kernel/sysfile.c` (or another kernel source).
  - Add user stub entry in `user/usys.pl` and prototype in `user/user.h`.

- Kernel change constraints (from `.github/instructions/kernel.instructions.md`):
  - Keep changes localized.
  - Maintain lock acquire/release pairing on all paths.
  - Validate user-kernel pointer/size arguments before use.
  - Avoid expensive/blocking work in trap/interrupt-sensitive paths.

- User-program change constraints (from `.github/instructions/user.instructions.md`):
  - Validate CLI args and print concise usage on invalid input.
  - Keep output deterministic.
  - Handle syscall failures explicitly and return non-zero on failure.

- Formatting:
  - Respect `.editorconfig`: C/H files use 2-space indentation, assembly (`.S`) uses 8, `Makefile` requires tabs.
