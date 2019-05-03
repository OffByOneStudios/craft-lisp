load("//build_tools:macros.bzl", "header_generator", "dll_generator", "entrypoint_generator")

header_generator(  
  deps = ["//types:headers", "@pegtl//:pegtl"]
)

dll_generator(
  packages = [
    "types",
  ],
  deps = [
    "@replxx//:replxx"
  ],
)

entrypoint_generator(
  name = "cult",
  packages = [
    "types",
  ],
  deps = [
    "@replxx//:replxx", 
    "@spdlog//:spdlog"
  ],
)
