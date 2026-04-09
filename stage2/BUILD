# stage2/BUILD
load("@rules_cc//cc:defs.bzl", "cc_binary")

# deps = ["//stage2/math:math_utils"]
#
# Label anatomy:
#   //stage2/math     — path to the package (directory with BUILD file)
#   :math_utils       — target name inside that BUILD file
#
# When the target name equals the last path component you can shorten it:
#   //stage2/math:math_utils  ==  //stage2/math
# (Bazel convention prefers the short form for public APIs)

cc_binary(
    name = "calculator",
    srcs = ["main.cc"],
    deps = ["//stage2/math:math_utils"],
)
