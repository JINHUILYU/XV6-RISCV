---
applyTo: "user/**/*.{c,h,S}"
description: "Use when editing xv6 user-space programs, including command behavior, argument handling, and output conventions."
---

# User Program Instructions

- Validate command-line arguments and print concise usage messages on invalid input.
- Keep program output deterministic and simple for testability.
- Handle syscall failures explicitly and return non-zero exit code when appropriate.
- Reuse helpers in user runtime where practical; avoid duplicated parsing logic.
- Keep each utility focused on one responsibility.
- For behavior changes, update or add a corresponding test in user programs when feasible.
