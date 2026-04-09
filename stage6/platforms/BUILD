# stage6/platforms/BUILD
# (same platform() rules as stage5, kept local to stage6 for clarity)

package(default_visibility = ["//visibility:public"])

platform(
    name = "linux_x86_64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:x86_64",
    ],
)

platform(
    name = "linux_arm64",
    constraint_values = [
        "@platforms//os:linux",
        "@platforms//cpu:aarch64",
    ],
)
