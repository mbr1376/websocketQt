
# Contributing to websocketQt

Thanks for taking the time to contribute! This repository contains a Qt-based WebSocket server and client, encryption helpers, logging utilities and tests. The guidelines below will help you get started and make high-quality contributions that are easy to review.

## Table of contents
- Getting started 🔧
- Project structure 📂
- Building & running ▶️
- Reporting issues 🐞
- Proposing changes (PRs) 🔀
- Branching & commit guidelines 📝
- Code style & tests ⚙️
- Continuous integration 🔁
- Security & license 🔒
- Developer tips 🔍
- Code of Conduct ✨

---

## Getting started 🔧
1. Fork this repo and clone your fork.
2. Install dependencies: Qt 6, CMake, a C++17+ compiler, Ninja or Make.
3. Recommended: use Qt Creator or a reproducible local toolchain that can find Qt6 (set `Qt6_DIR` if needed).
4. Build a clean baseline and run tests locally (see "Building & running").

## Project structure 📂
- `websocketServer/` — server implementation
- `websocketclient/` — client sample (UI) and sample code
- `encrypt/` — encryption helpers and tests
- `log/` — logging utilities
- `db/` — sample DB files
- `wss/` — TLS/WSS example notes and helpers

## Building & running ▶️
### CMake (recommended)

```bash
mkdir -p build && cd build
cmake -S .. -B .
cmake --build . --parallel
# Build and run the server (target may be located under build tree):
cmake --build . --target websocketServer
./websocketServer/websocketServer
# Build and run the client sample:
cmake --build . --target WebSocketClientSample
./websocketclient/WebSocketClientSample
# Run tests:
ctest -V
```

### qmake (alternate)

```bash
cd websocketclient
qmake && make && make test
```

Notes:
- For WSS/TLS example see `wss/runPrivate.txt` which contains instructions for generating local certs and running the example.
- If using Qt Creator, open the top-level `CMakeLists.txt`.

## Reporting issues 🐞
- Search existing issues before opening a new one.
- Provide: clear title, steps to reproduce, expected vs actual behavior, logs, OS & Qt versions, and a minimal repro if possible.
- Use labels: `bug`, `enhancement`, `question`, `security` (sensitive issues should be reported privately).

## Proposing changes (Pull Requests) 🔀
- Create a branch from `main` and name it `feat/...`, `fix/...` or `docs/...`.
- Keep PRs focused (one logical change per PR).
- In the PR description include related issue(s), build + run steps to verify, and test instructions.
- Add or update unit tests for behavioral changes.
- Ensure all CI checks pass before requesting review.

## Branching & commit guidelines 📝
- Branch naming: `feat/short-description`, `fix/short-description`.
- Commit message format:
  - Short summary (<= 50 chars)
  - Blank line
  - Optional detailed explanation and issue reference (e.g., `Fixes #123`)
- Rebase and squash WIP commits when requested by reviewers.

## Code style & tests ⚙️
- Language: C++17 or later; follow Qt idioms (`QString`, `QByteArray`, `QSharedPointer`, signals/slots).
- Headers: prefer `#pragma once` unless the project has a different convention.
- Use `clang-format` if the project provides a style file. Add a `.clang-format` if needed.
- Tests:
  - Add tests under the relevant module (e.g., `encrypt/tests/`).
  - Use Qt Test or the repo's test harness. Keep tests fast and self-contained.
  - Run tests locally: `ctest -V` or run the test executable directly.

Checklist before opening a PR:
- Code builds on CI (or documented platform differences)
- Tests (existing + new) pass locally
- Formatting/linting applied
- Documentation or README updates included if behavior changed

## Continuous integration 🔁
- CI runs on each PR and push (build + tests).
- If you add platforms or tests, update the CI workflows in `.github/workflows/` or other CI config used by the repo.

## Security & license 🔒
- Never commit secrets (keys, passwords). Use environment variables or CI secrets.
- Report security issues privately by opening an issue marked `security` or contacting the maintainers.
- By contributing you agree to license your contributions under this project's license (see `LICENSE`).

## Developer tips 🔍
- Encryption tests: see `encrypt/tests/encrypt_roundtrip.cpp` for examples.
- Local WSS testing: follow instructions in `wss/runPrivate.txt` to generate certs and run server locally.
- Logs: check `log/` for the logger helpers and usage examples.
- DB: sample DB files are in `db/`; avoid committing production DB files.

## Code of Conduct ✨
- Be respectful, collaborative, and constructive. Follow the project's Code of Conduct (see `CODE_OF_CONDUCT.md` if present).

---

If you'd like, I can open a branch, apply this change, and prepare a PR for you — should I go ahead and create a branch and commit this change? 🔧