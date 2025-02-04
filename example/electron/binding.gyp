{
  "targets": [
    {
      "target_name": "filerix",
      "sources": [
        "src/lib/wrapper.cc",
      ],
      "include_dirs": [
        "<!(pkg-config --cflags filerix)",
        "<!(node -p \"require('node-addon-api').include\")"
      ],
      "dependencies": [
        "<!(node -p \"require('node-addon-api').gyp\")",
        "<!(node -p \"require('node-addon-api').targets\"):node_addon_api",
      ],
      "libraries": ["<!(pkg-config --libs filerix)"],
    }
  ]
}
