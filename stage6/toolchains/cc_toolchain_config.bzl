# stage6/toolchains/cc_toolchain_config.bzl
#
# KEY CONCEPT — cc_toolchain_config
# ────────────────────────────────────
# This is the Bazel equivalent of a CMake toolchain file.
#
# CMake toolchain file for cross-compilation looks like:
#   set(CMAKE_SYSTEM_NAME    Linux)
#   set(CMAKE_SYSTEM_PROCESSOR aarch64)
#   set(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
#   set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)
#   set(CMAKE_AR           aarch64-linux-gnu-ar)
#   ...
#
# In Bazel that information is a Starlark function returning
# CcToolchainConfigInfo. The provider tells the C++ rules:
#   - what binary to call for each action (compile, link, archive …)
#   - what flags to pass by default
#   - what features are available (opt, dbg, pic, lto, sanitizers, …)

# cc_toolchain_config_lib provides struct constructors for the API.
# These replaced cc_common.create_tool_path() etc. starting in Bazel 7.
load(
    "@rules_cc//cc:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)
load("@rules_cc//cc:defs.bzl", "cc_common")

_CPP_COMPILE_ACTIONS = [
    "c++-compile",
    "c++-header-parsing",
    "c++-module-compile",
    "linkstamp-compile",
]

_ALL_COMPILE_ACTIONS = [
    "assemble",
    "preprocess-assemble",
    "c-compile",
] + _CPP_COMPILE_ACTIONS

_LINK_ACTIONS = [
    "c++-link-executable",
    "c++-link-dynamic-library",
    "c++-link-nodeps-dynamic-library",
]

def _linux_arm64_toolchain_config_impl(ctx):
    # ── Tool paths ──────────────────────────────────────────────────
    # Maps each Bazel tool name to the actual binary on the host.
    # Equivalent to set(CMAKE_CXX_COMPILER ...) etc.
    tool_paths_list = [
        tool_path(name = "gcc",     path = "/usr/bin/aarch64-linux-gnu-gcc"),
        tool_path(name = "g++",     path = "/usr/bin/aarch64-linux-gnu-g++"),
        tool_path(name = "ar",      path = "/usr/bin/aarch64-linux-gnu-ar"),
        tool_path(name = "cpp",     path = "/usr/bin/aarch64-linux-gnu-cpp"),
        tool_path(name = "gcov",    path = "/usr/bin/aarch64-linux-gnu-gcov"),
        tool_path(name = "ld",      path = "/usr/bin/aarch64-linux-gnu-ld"),
        tool_path(name = "nm",      path = "/usr/bin/aarch64-linux-gnu-nm"),
        tool_path(name = "objcopy", path = "/usr/bin/aarch64-linux-gnu-objcopy"),
        tool_path(name = "objdump", path = "/usr/bin/aarch64-linux-gnu-objdump"),
        tool_path(name = "strip",   path = "/usr/bin/aarch64-linux-gnu-strip"),
    ]

    # ── Features ────────────────────────────────────────────────────
    # A "feature" is a named group of flags Bazel can enable/disable.
    # Built-ins: opt, dbg, fastbuild, pic, lto …
    # Custom: anything you define here.
    #
    # Equivalent to add_compile_options() / add_link_options() in CMake
    # but organised into named, composable groups.

    default_compile_flags = feature(
        name = "default_compile_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _ALL_COMPILE_ACTIONS,
                flag_groups = [flag_group(flags = [
                    
                    "-no-canonical-prefixes",
                    "-fstack-protector",
                    "-Wall",
                ])],
            ),
            flag_set(
                actions = _CPP_COMPILE_ACTIONS,
                flag_groups = [flag_group(flags = [
                    "-std=c++17",
                    "-isystem", "/usr/aarch64-linux-gnu/include/c++/13",
                    "-isystem", "/usr/aarch64-linux-gnu/include/c++/13/aarch64-linux-gnu",
                    "-isystem", "/usr/aarch64-linux-gnu/include",
                ])],
            ),
        ],
    )

    # No extra link flags needed — aarch64-linux-gnu-gcc has its
    # multiarch library search paths built in.
    default_link_flags = feature(
        name = "default_link_flags",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = _LINK_ACTIONS,
                flag_groups = [flag_group(flags = ["-lstdc++"])],
            ),
        ],
    )
    # opt / dbg / fastbuild — same concept as CMAKE_BUILD_TYPE.
    opt = feature(
        name = "opt",
        flag_sets = [
            flag_set(
                actions = _ALL_COMPILE_ACTIONS,
                flag_groups = [flag_group(flags = ["-O2", "-DNDEBUG"])],
            ),
        ],
    )

    dbg = feature(
        name = "dbg",
        flag_sets = [
            flag_set(
                actions = _ALL_COMPILE_ACTIONS,
                flag_groups = [flag_group(flags = ["-g", "-O0"])],
            ),
        ],
    )

    return cc_common.create_cc_toolchain_config_info(
        ctx                  = ctx,
        toolchain_identifier = "aarch64-linux-gnu-gcc",
        target_system_name   = "aarch64-linux-gnu",
        target_cpu           = "aarch64",
        target_libc          = "glibc",
        host_system_name     = "x86_64-linux-gnu",
        compiler             = "gcc",
        abi_version          = "aarch64",
        abi_libc_version     = "glibc",
        tool_paths           = tool_paths_list,
        features             = [default_compile_flags, default_link_flags, opt, dbg],
        # Tell Bazel which include dirs the compiler adds silently,
        # so it doesn't report them as undeclared dependencies.
        cxx_builtin_include_directories = [
            "/usr/aarch64-linux-gnu/include",
            "/usr/aarch64-linux-gnu/include/c++/13",
            "/usr/aarch64-linux-gnu/include/c++/13/aarch64-linux-gnu",
            "/usr/lib/gcc-cross/aarch64-linux-gnu/13/include",
            "/usr/lib/gcc-cross/aarch64-linux-gnu/13/include-fixed",
        ],
    )

# A rule with no special attributes — it just calls the impl function
# and returns CcToolchainConfigInfo.
linux_arm64_toolchain_config = rule(
    implementation = _linux_arm64_toolchain_config_impl,
    attrs = {},
)
