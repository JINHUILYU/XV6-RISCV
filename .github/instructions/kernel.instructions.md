---
applyTo: "kernel/**/*.{c,h,S}"
description: "Use when editing xv6 kernel code, including locks, memory management, traps, and syscall paths."
---

# Kernel Coding Instructions

- Keep changes minimal and localized; avoid broad refactors unless explicitly requested.
- Ensure lock acquire/release pairing on all success and error paths.
- Validate pointer and size arguments before use, especially on user-kernel boundaries.
- Preserve xv6 style and naming consistency with nearby code.
- Prefer clear, small functions; split logic that becomes hard to reason about.
- Do not introduce blocking or expensive work in interrupt/trap-sensitive paths.
- When changing behavior, add or update a user-level test program when feasible.
