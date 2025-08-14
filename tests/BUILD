load("@rules_cc//cc:cc_test.bzl", "cc_test")

cc_test(
    name = "test_main",
    srcs = ["test_main.cpp"],
    deps = ["//source/common:libcommon", 
    "//source/parse:libparse", "//source/fileio:libfileio", 
            "//source/alsaout:libalsaout"],
                linkopts = ["-lasound"],
    copts = ["-g", "-O0"],  # Add debug info and disable optimization
)

cc_test(
    name = "test_bitstream",
    srcs = ["test_bitstream.cpp"],
    deps = ["//source/common:libcommon", 
            "//source/fileio:libfileio"],
    copts = ["-g", "-O0"],  # Add debug info and disable optimization
)
