# stage6/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

# Build natively:
#   bazel build //stage6:hello
#
# Cross-compile for ARM64:
#   bazel build --platforms=//stage6/platforms:linux_arm64 //stage6:hello
#
# Inspect the result:
#   file bazel-bin/stage6/hello
#   # → ELF 64-bit LSB executable, ARM aarch64  (when cross-compiled)
#   # → ELF 64-bit LSB executable, x86-64       (when native)

cc_binary(
    name = "hello",
    srcs = ["hello.cc"],
)
