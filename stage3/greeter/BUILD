# stage3/greeter/BUILD
load("@rules_cc//cc:defs.bzl", "cc_library")

# TRANSITIVE DEPS:
# 'greeter_lib' deps on 'strings'.  The binary in stage3/BUILD only needs
# to list //stage3/greeter:greeter_lib — it will NOT need to list
# //stage3/strings:strings because that is a private implementation detail
# of greeter.
#
# Compare with CMake:
#   target_link_libraries(greeter_lib PRIVATE strings)
#   target_link_libraries(app PRIVATE greeter_lib)  # strings is automatic

cc_library(
    name = "greeter_lib",
    srcs = ["greeter.cc"],
    hdrs = ["greeter.h"],
    deps = ["//stage3/strings:strings"],
    visibility = ["//visibility:public"],
)
