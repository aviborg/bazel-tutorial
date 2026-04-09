# stage4/math/BUILD
load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

# cc_test — builds and registers a test binary.
#
# 'size' hints Bazel about expected runtime for parallelism scheduling:
#   "small"   < 1 min   (unit tests)
#   "medium"  < 5 min   (integration tests)
#   "large"   < 15 min
#   "enormous"           (system tests)
#
# RUN TESTS:
#   bazel test //stage4/math:math_utils_test
#   bazel test //stage4/...          # all tests under stage4
#   bazel test //...                 # all tests in the workspace

cc_library(
    name = "math_utils",
    srcs = ["math_utils.cc"],
    hdrs = ["math_utils.h"],
    visibility = ["//visibility:public"],
)

cc_test(
    name = "math_utils_test",
    size = "small",
    srcs = ["math_utils_test.cc"],
    deps = [
        ":math_utils",
        "@googletest//:gtest_main",
    ],
)
