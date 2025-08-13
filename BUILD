load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

cc_binary(
    name = "player",
    srcs = ["source/main.cpp"],
    deps = ["//source/alsaout:libalsaout",
                "//source/common:libcommon", "//source/parse:libparse", 
                "//source/fileio:libfileio"],
    copts = ["-g", "-O0"],
)