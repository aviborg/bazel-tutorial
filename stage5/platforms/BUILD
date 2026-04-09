# stage5/platforms/BUILD
#
# KEY CONCEPT — platform()
# ────────────────────────────
# A platform is a named set of constraints describing a machine.
# Think of it as a typed struct:  { os: linux, cpu: aarch64 }
#
# Bazel uses this to:
#  1. Resolve select() branches at analysis time.
#  2. Select the right toolchain (compiler, linker, sysroot).
#
# Compare with CMake toolchain files:
#   set(CMAKE_SYSTEM_NAME Linux)
#   set(CMAKE_SYSTEM_PROCESSOR aarch64)
#   set(CMAKE_C_COMPILER    aarch64-linux-gnu-gcc)
#   ...
#
# In Bazel the platform declaration is separate from the toolchain —
# platform says WHAT you want to build for; a toolchain says HOW.
#
# BUILT-IN PLATFORMS (always available, no declaration needed):
#   @platforms//host              — the machine running Bazel
#   @local_config_platform//:host — detected at build time
#
# Pass a custom platform at the command line:
#   bazel build --platforms=//stage5/platforms:linux_arm64 //stage5:info
#
# (Cross-compilation also requires a registered cc_toolchain for that
#  platform — see the toolchain section in README.md)

package(default_visibility = ["//visibility:public"])

# ── Linux x86-64 (the host in this tutorial) ──────────────────────
platform(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

# ── Linux ARM64 (e.g. Raspberry Pi 4, AWS Graviton) ───────────────
platform(
    name = "linux_arm64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)

# ── macOS Apple Silicon ────────────────────────────────────────────
platform(
    name = "macos_arm64",
    constraint_values = [
        "@platforms//os:macos",
        "@platforms//cpu:aarch64",
    ],
)

# ── Windows x86-64 ────────────────────────────────────────────────
platform(
    name = "windows_x86_64",
    constraint_values = [
        "@platforms//os:windows",
        "@platforms//cpu:x86_64",
    ],
)

# ── Custom constraint: hardware variant ───────────────────────────
# You can define your own constraint axes beyond OS and CPU.
# Example: distinguish embedded boards by memory size.

constraint_setting(name = "memory_tier")

constraint_value(
    name = "memory_small",   # < 512 MB
    constraint_setting = ":memory_tier",
)

constraint_value(
    name = "memory_large",   # >= 4 GB
    constraint_setting = ":memory_tier",
)

# An embedded target: Linux ARM + small memory
platform(
    name = "embedded_arm64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
        ":memory_small",
    ],
)
