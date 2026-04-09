# Bazel C++ Tutorial

A hands-on guide for C++ developers transitioning from Make or CMake.

```
bazel-tutorial/
├── MODULE.bazel          ← workspace root (replaces old WORKSPACE file)
├── .bazelrc              ← default build flags
├── stage1/               ← cc_binary: the minimal build
├── stage2/               ← cc_library + cc_binary in the same package
├── stage3/               ← multiple packages with transitive deps
├── stage4/               ← cc_test with GoogleTest
├── stage5/               ← platform targets and select()
├── stage6/               ← cross-compilation with cc_toolchain
└── stage7/               ← shared caching: disk cache and remote cache
```

---

## Core Concepts

| What you need | Bazel concept | You may know it as |
|---|---|---|
| Workspace root | `MODULE.bazel` | `Makefile` / `CMakeLists.txt` at project root |
| Sub-project | directory with a `BUILD` file (a *package*) | `add_subdirectory()` / recursive `Makefile` |
| Executable | `cc_binary(name="foo", …)` | `add_executable` / `$(CC) -o foo` |
| Library | `cc_library(name="bar", …)` | `add_library` / archive rule |
| Dependency | `deps = [":bar"]` or `deps = ["//path:bar"]` | `target_link_libraries` / `-l` flag |
| Test | `cc_test(name="…_test", …)` | CTest / a plain executable |
| External dep | `bazel_dep()` in `MODULE.bazel` | `FetchContent` / `find_package` / `pkg-config` |
| Build output | `bazel-bin/` | `build/` directory |
| Incremental build | SHA-based hermetic cache | `make` timestamp comparison |

---

## Cheat Sheet

```bash
# Build a single target
bazel build //stage1:hello

# Build everything under a package tree
bazel build //stage3/...

# Build everything in the workspace
bazel build //...

# Run a binary directly
bazel run //stage1:hello

# Run a single test
bazel test //stage4/math:math_utils_test

# Run all tests
bazel test //...

# Show the dependency graph (DOT format)
bazel query 'deps(//stage3:app)' --output graph | dot -Tpng > deps.png

# Find which targets depend on a library (reverse deps)
bazel query 'rdeps(//..., //stage3/strings:strings)'

# Clean build outputs
bazel clean

# Clean including the action cache
bazel clean --expunge
```

---

## Label Syntax

```
//stage2/math:math_utils
^      ^         ^
|      |         └─ target name (the 'name' field in the BUILD rule)
|      └─────────── package path (relative to workspace root)
└────────────────── '//' = workspace root
```

**Shorthand**: when the target name equals the last directory component you can omit it:
```
//stage2/math:math_utils  ==  //stage2/math
```

**Same-package reference**:
```
deps = [":math_utils"]   # no '//' prefix, colon only
```

---

## Stage 1 — Hello World (`cc_binary`)

```bash
cd /path/to/bazel-tutorial
bazel build //stage1:hello
bazel run   //stage1:hello
# binary also at: bazel-bin/stage1/hello
```

---

## Stage 2 — Library + Binary (`cc_library`)

```bash
bazel build //stage2:calculator
bazel run   //stage2:calculator
```

Key points:
- `hdrs` = headers other targets may `#include`.
- `srcs` = `.cc` files + private headers.
- `visibility = ["//visibility:public"]` opens the target to all packages.

---

## Stage 3 — Multi-Package Project

```
stage3/
├── BUILD            ← cc_binary: app
├── main.cc
├── greeter/
│   ├── BUILD        ← cc_library: greeter_lib  (deps on strings)
│   ├── greeter.h
│   └── greeter.cc
└── strings/
    ├── BUILD        ← cc_library: strings
    ├── strings.h
    └── strings.cc
```

```bash
bazel build //stage3/...   # build all at once
bazel run   //stage3:app
```

Dependency graph: `app` → `greeter_lib` → `strings`

Bazel propagates transitive link dependencies automatically — `app`'s
`deps` only needs `greeter_lib`.

---

## Stage 4 — Unit Testing (`cc_test` + GoogleTest)

GoogleTest is declared via `bazel_dep()` in `MODULE.bazel` — no manual
downloading needed.

> **Note (Bazel 7+):** `cc_binary`, `cc_library`, and `cc_test` are no longer
> auto-loaded. You must add a `load()` statement at the top of every BUILD file:
> ```python
> load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")
> ```
> And declare `rules_cc` in `MODULE.bazel`:
> ```python
> bazel_dep(name = "rules_cc", version = "0.2.17")
> ```

```bash
# Run the test (fetches GoogleTest on first run)
bazel test //stage4/math:math_utils_test

# Verbose output
bazel test //stage4/math:math_utils_test --test_output=all

# Run all tests in workspace
bazel test //...
```

Test output is stored in `bazel-testlogs/`.

---

## Stage 5 — Platform Targets and `select()`

`select()` makes any rule attribute conditional on the target platform.
It works on `srcs`, `hdrs`, `deps`, `copts`, `linkopts`, `defines`, and more.

```python
srcs = select({
    "@platforms//os:linux":   ["impl_linux.cc"],
    "@platforms//os:macos":   ["impl_macos.cc"],
    "@platforms//os:windows": ["impl_windows.cc"],
    "//conditions:default":   ["impl_linux.cc"],
})
```

A `platform()` rule names a set of constraints (OS, CPU, or custom):

```python
platform(
    name = "linux_arm64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)
```

Pass it at the command line to change the target platform:

```bash
bazel build --platforms=//stage5/platforms:linux_arm64 //stage5:info
```

Built-in constraint axes from `@platforms`:

| OS | CPU |
|---|---|
| `@platforms//os:linux` | `@platforms//cpu:x86_64` |
| `@platforms//os:macos` | `@platforms//cpu:aarch64` |
| `@platforms//os:windows` | `@platforms//cpu:arm` |

---

## Stage 6 — Cross-Compilation (`cc_toolchain`)

A toolchain tells Bazel *how* to build for a given platform: which compiler
binaries to invoke, which flags to pass, and which system include paths exist.

Three pieces wire it together:

```
cc_toolchain_config  (the "recipe" — compiler paths, features, flags)
       ↓
cc_toolchain         (the "wrapper" — binds config to a set of compiler files)
       ↓
toolchain            (the "matchmaker" — activates cc_toolchain for a platform)
       ↓
register_toolchains  (in MODULE.bazel — makes Bazel aware of it globally)
```

Once registered, toolchain selection is automatic:

```bash
# Native build
bazel build //stage6:hello

# Cross-compile → ARM64
bazel build --platforms=//stage6/platforms:linux_arm64 //stage6:hello

# Release build (activates the 'opt' feature: -O2 -DNDEBUG)
bazel build --platforms=//stage6/platforms:linux_arm64 -c opt //stage6:hello

# Debug build (activates the 'dbg' feature: -g -O0)
bazel build --platforms=//stage6/platforms:linux_arm64 -c dbg //stage6:hello
```

`-c opt` / `-c dbg` / `-c fastbuild` are the Bazel equivalents of build-type
flags you would pass to your build system to select an optimisation level.

---

## Stage 7 — Shared Caching

### How the cache key works

Every compiled action has a cache key derived from:

- The content hash of all input source files
- The compiler binary hash
- All flags (copts, defines, …)
- The Bazel version

Branch name, commit hash, and developer identity are **not** part of the key.
Two developers on two different branches compiling the same unchanged file
with the same flags share the exact same cache entry.

```
main      ──●──────────────────●── CI compiles 50 libraries → cache populated
              \                /
feature-x  ───●── touch 2 files    developer: only 2 libraries recompile,
                                   48 are fetched from cache
```

---

### Option A — Disk cache (zero server setup)

The disk cache stores outputs in a local directory using the same
content-addressed format as the remote cache. It survives `bazel clean`
(only `bazel clean --expunge` removes it).

**Single developer** — persist cache across cleans:
```bash
# Add to ~/.bazelrc (not committed — path is machine-specific)
build --disk_cache=~/.cache/bazel-disk
```

**Small team on a shared machine or NFS mount** — point everyone at the same path:
```bash
# Add to the project .bazelrc
build --disk_cache=/mnt/nfs/shared/bazel-cache
```

Bazel handles concurrent reads and writes safely — each entry is an
immutable content-addressed file, so there are no lock conflicts.

Optional size / age limits:
```bash
build --experimental_disk_cache_gc_max_size=20g
build --experimental_disk_cache_gc_max_age=14d
```

See [`stage7/.bazelrc.disk_cache`](stage7/.bazelrc.disk_cache) for annotated examples.

---

### Option B — Remote HTTP cache (large teams, CI/CD)

For 50+ developers the recommended setup is a central HTTP cache server.
`bazel-remote` is the most widely used open-source implementation.

**Start the server** (once, on any machine your team can reach):
```bash
cd stage7 && docker compose up -d
```

**Each developer** adds one line to `~/.bazelrc`:
```bash
build --remote_cache=http://CACHE_HOST:8080
build --remote_local_fallback   # keep building if server is unreachable
```

**CI** uploads results; developers download them:
```bash
# In CI pipeline:
bazel build //... --remote_cache=http://CACHE_HOST:8080 --config=ci

# Developer machine (download-only):
bazel build //... --remote_cache=http://CACHE_HOST:8080 --config=dev
```

See [`stage7/.bazelrc.remote`](stage7/.bazelrc.remote) for the full
`--config=ci` / `--config=dev` split and timeout settings.

---

### Combining both caches

You can use disk cache and remote cache at the same time. Bazel checks in order:

```
1. In-memory cache (current invocation only)
2. --disk_cache  (local disk — microsecond access)
3. --remote_cache (network — millisecond access)
4. Compile locally and write results back to both
```

```bash
build --disk_cache=~/.cache/bazel-disk
build --remote_cache=http://CACHE_HOST:8080
```

---

### Cache in numbers (measured on this machine)

| Scenario | Compiled actions | Elapsed |
|---|---|---|
| Cold build, no cache | 8 | 22 s |
| Disk cache hit (same machine) | 0 | < 1 s |
| Remote cache hit (cold local disk) | 0 | 18 s |

---

## Key Things to Know

1. **Hermetic headers** — every header must be listed in `hdrs` or `srcs`.
   Forgetting one is a build error, not a silent include path leak.

2. **No separate configure step** — there is no equivalent of a project
   generation command. `bazel build //...` is all you need.

3. **Workspace-rooted include paths** — always write
   `#include "stage2/math/math_utils.h"`, never `#include "../math/math_utils.h"`.

4. **Strict visibility** — targets are private to their package by default.
   Sharing is an explicit opt-in with `visibility = ["//visibility:public"]`.

5. **Reproducible by design** — every action is a pure function of its inputs.
   Unchanged targets are never recompiled, and the cache is shareable across
   machines for distributed builds.

---

## MODULE.bazel.lock

`MODULE.bazel.lock` is Bazel's dependency lockfile. It is the direct equivalent
of `package-lock.json` (npm), `Cargo.lock` (Rust), or `poetry.lock` (Python).

### What it records

`MODULE.bazel` states your *intentions* — the versions you asked for:

```python
bazel_dep(name = "googletest", version = "1.17.0.bcr.2")
bazel_dep(name = "rules_cc",   version = "0.2.17")
```

The lockfile records what Bazel *actually resolved*, including every transitive
dependency and the SHA-256 hash of each file downloaded from the registry:

```
googletest 1.17.0.bcr.2
  └─ abseil-cpp 20250127.1      (hash: c4a89e…)
  └─ re2 2025-08-12.bcr.1       (hash: …)
  └─ rules_cc 0.2.8             (hash: …)
```

Without a lockfile, two developers running `bazel build` on different days could
silently pick up a newly published version and get different binaries from the
same source tree.

### Commit it

Always commit `MODULE.bazel.lock` to version control. It makes the build
reproducible across all machines, branches, and CI runs — the same sources
always produce the same binary.

### Never edit it by hand

Bazel regenerates the lockfile automatically whenever you run any build command
after changing `MODULE.bazel`. To explicitly refresh it:

```bash
# Recalculate and rewrite the lockfile
bazel mod deps --lockfile_mode=update

# Check whether the lockfile is still up to date (exits non-zero if stale)
bazel mod deps --lockfile_mode=error
```

### Upgrade a dependency

```bash
# 1. Edit MODULE.bazel — bump the version number
# 2. Let Bazel resolve and rewrite the lockfile
bazel mod deps --lockfile_mode=update

# 3. Inspect what changed
git diff MODULE.bazel.lock

# 4. Commit both files together
git add MODULE.bazel MODULE.bazel.lock
git commit -m "chore: upgrade googletest to 1.17.0.bcr.2"
```

### Inspect the resolved dependency graph

```bash
# Print the full resolved graph
bazel mod graph

# Show only direct deps
bazel mod deps

# Explain why a specific version was chosen
bazel mod explain --module googletest
```

