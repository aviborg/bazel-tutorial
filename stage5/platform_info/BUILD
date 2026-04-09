# stage5/platform_info/BUILD
#
# KEY CONCEPT — select()
# ────────────────────────
# select() is Bazel's way to make any rule attribute conditional on
# the *target platform* (or other configuration flags).
#
# Compare with CMake:
#   if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#     target_sources(lib PRIVATE platform_info_linux.cc)
#   elseif(...)
#   ...
#   endif()
#
# In Bazel you write that as a select() directly inside the rule.
# Bazel evaluates it at analysis time — before compiling anything.
#
# BUILT-IN PLATFORM CONSTRAINTS (from the @platforms module):
#
#   OS constraints:            CPU constraints:
#   @platforms//os:linux       @platforms//cpu:x86_64
#   @platforms//os:macos       @platforms//cpu:aarch64
#   @platforms//os:windows     @platforms//cpu:arm
#                              @platforms//cpu:riscv64
#
# "//conditions:default" is the fallback — like 'else' in an if/else.

load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "platform_info",
    hdrs = ["platform_info.h"],

    # select() on srcs:
    # Bazel picks exactly ONE branch based on the active platform.
    srcs = select({
        "@platforms//os:linux":   ["platform_info_linux.cc"],
        "@platforms//os:macos":   ["platform_info_macos.cc"],
        "@platforms//os:windows": ["platform_info_windows.cc"],
        "//conditions:default":   ["platform_info_linux.cc"],
    }),

    visibility = ["//visibility:public"],
)
