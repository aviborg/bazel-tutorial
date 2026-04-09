# stage5/BUILD
#
# ── select() on copts ─────────────────────────────────────────────
# select() works on ANY rule attribute: srcs, hdrs, deps, copts,
# linkopts, defines, data, …
#
# Here we use it to add platform-specific compiler flags.
# CMake equivalent:
#   if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#     target_compile_options(info PRIVATE -D_GNU_SOURCE)
#   endif()

load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "info",
    srcs = ["main.cc"],
    deps = ["//stage5/platform_info:platform_info"],

    # Platform-conditional compile flags.
    copts = select({
        "@platforms//os:linux":   ["-D_GNU_SOURCE"],
        "@platforms//os:macos":   ["-D_DARWIN_C_SOURCE"],
        "@platforms//os:windows": ["/W4"],
        "//conditions:default":   [],
    }),
)
