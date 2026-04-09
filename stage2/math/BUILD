# stage2/math/BUILD
load("@rules_cc//cc:defs.bzl", "cc_library")

# cc_library — equivalent to add_library() in CMake.
#
# 'hdrs' vs 'srcs':
#   hdrs  = headers exposed to dependents (#include-able from outside)
#   srcs  = implementation files + private headers (not exposed)
#
# 'visibility':
#   ["//visibility:public"]   — any package can use this target
#   ["//stage2:__subpackages__"] — only packages under stage2/
#   ["//stage2:__pkg__"]      — only targets in the same BUILD file
#
# If you omit visibility, the default is private to the package.

cc_library(
    name = "math_utils",
    srcs = ["math_utils.cc"],
    hdrs = ["math_utils.h"],
    visibility = ["//visibility:public"],
)
