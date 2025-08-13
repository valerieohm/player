load("@rules_cc//cc:cc_library.bzl", "cc_library")

cc_library(
    name = "libparse",
    srcs = glob(["*.cpp"]),
    hdrs = glob(["*.hpp"]),
    includes = ["include"],
    deps = ["//source/common:libcommon"],
    visibility = ["//visibility:public"],
    copts = ["-g", "-O0"],
    linkstatic = 1,

)
