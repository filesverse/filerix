{
  "targets": [
    {
      "target_name": "libfm",
      "sources": [
        "wrapper.cc",
      ],
      "include_dirs": ["<!(node -p \"require('node-addon-api').include\")"],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")",
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api",
      ],
      "libraries": ["/usr/local/lib64/libfm.so"],
    }
  ]
}
