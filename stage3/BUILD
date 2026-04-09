# stage3/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

# Build everything in stage3 at once:
#   bazel build //stage3/...
#
# The '...' glob means "this package and all packages below it".
# Equivalent to a recursive CMake add_subdirectory() walk.

cc_binary(
    name = "app",
    srcs = ["main.cc"],
    deps = ["//stage3/greeter:greeter_lib"],
)
