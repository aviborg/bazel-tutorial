# stage1/BUILD
#
# In Bazel 7+ C++ rules are NOT auto-loaded. You must load them explicitly.
# This is similar to having to `find_package(...)` in CMake.
load("@rules_cc//cc:defs.bzl", "cc_binary")

# KEY CONCEPT — BUILD file
# --------------------------
# Every directory that Bazel should know about needs a BUILD file.
# A directory with a BUILD file is called a "package".
# This is analogous to a CMakeLists.txt inside a subdirectory.
#
# A BUILD file contains *rules*.  Rules have:
#   name  — the target name  (like a Makefile target)
#   srcs  — source files
#   hdrs  — public headers (for libraries)
#   deps  — dependencies on other targets
#
# BUILD RULE SYNTAX KEY:
#   //stage1:hello        — absolute label  (//  = workspace root)
#   :hello                — relative label  (same package)
#   @googletest//:gtest   — external label  (@  = external repo)

cc_binary(
    name = "hello",
    srcs = ["hello.cc"],
)
