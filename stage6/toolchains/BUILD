# stage6/toolchains/BUILD
#
# Registers a complete cross-compilation toolchain for Linux ARM64.
#
# ── The four pieces you always need ──────────────────────────────
#
# 1. linux_arm64_toolchain_config  (from .bzl file)
#       ↓  produces CcToolchainConfigInfo
#
# 2. cc_toolchain()
#       ↓  known as a "C++ toolchain" — wraps config + compiler filegroups
#
# 3. toolchain()
#       ↓  wraps cc_toolchain with platform constraints (the "matchmaker")
#
# 4. register_toolchains() in MODULE.bazel
#       ↓  makes Bazel aware of it globally
#
# ── Analogy to CMake ─────────────────────────────────────────────
#
# CMake cross-compile flow:
#   cmake -DCMAKE_TOOLCHAIN_FILE=arm64.cmake -B build && cmake --build build
#
# Bazel cross-compile flow:
#   bazel build --platforms=//stage6/platforms:linux_arm64 //stage6:hello
#
# Bazel picks the toolchain automatically from register_toolchains().
# No per-build flag pointing at a file — the platform drives selection.

load("@rules_cc//cc:defs.bzl", "cc_toolchain")
load(":cc_toolchain_config.bzl", "linux_arm64_toolchain_config")

# ── Step 1: instantiate the config rule ──────────────────────────
# This calls _linux_arm64_toolchain_config_impl() and produces
# CcToolchainConfigInfo.

linux_arm64_toolchain_config(name = "linux_arm64_config")

# ── Step 2: cc_toolchain — wraps the config + lists compiler files ──
# all_files / compiler_files / linker_files:
#   These are filegroups listing the actual binaries Bazel must
#   make available inside the sandbox before invoking them.
#   Using ":empty" is fine when the compiler binaries are from the
#   host PATH (absolute paths in tool_paths) — Bazel won't sandbox
#   system tools by default with --noincompatible_sandbox_hermetic_tmp.
#
#   For a fully hermetic toolchain you'd package the cross-compiler
#   as a Bazel-managed download and point these at its filegroup.

filegroup(name = "empty")

cc_toolchain(
    name = "linux_arm64_cc_toolchain",
    toolchain_identifier = "aarch64-linux-gnu-gcc",
    toolchain_config = ":linux_arm64_config",
    all_files       = ":empty",
    compiler_files  = ":empty",
    dwp_files       = ":empty",
    linker_files    = ":empty",
    objcopy_files   = ":empty",
    strip_files     = ":empty",
    supports_param_files = 0,
)

# ── Step 3: toolchain() — the "matchmaker" ─────────────────────────
# toolchain() is the generic Bazel wrapper (not cc-specific).
# It says:
#   "When Bazel is building FOR linux_arm64 ON linux_x86_64,
#    use linux_arm64_cc_toolchain as the C++ toolchain."
#
# toolchain_type:       what kind of thing this provides (C++ rules)
# target_compatible_with: the TARGET platform this applies to
# exec_compatible_with:  the HOST platform this compiler runs on
#
# Leaving exec_compatible_with empty means "works on any host".

toolchain(
    name = "linux_arm64_toolchain",
    toolchain_type = "@rules_cc//cc:toolchain_type",
    toolchain = ":linux_arm64_cc_toolchain",
    target_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
    # Constrain to x86_64 Linux hosts (where this cross-compiler is installed).
    exec_compatible_with = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
    visibility = ["//visibility:public"],
)
