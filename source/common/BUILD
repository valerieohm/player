load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "libcommon",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),
    includes = ["."],
    visibility = ["//visibility:public"],
    copts = ["-g", "-O0"],
    linkstatic = 1,
)
