{
  "targets": [
    {
      "target_name": "BarCodes",
      "sources": [ "BarCodes.cc" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs": [
        "<(module_root_dir)/zxing-cpp/core/src",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'libraries': [
        "-l<(module_root_dir)/zxing-cpp.release\\core\\Release\\ZXing.lib"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'conditions': [
        ["OS=='win'", {
          "defines": [
            "_HAS_EXCEPTIONS=1"
          ],
          "configurations":{
            'Release': {
              'msvs_settings': {
                'VCCLCompilerTool': {
                  'AdditionalOptions': [ '-std:c++17', ],
                  'RuntimeLibrary': '2'
                  # 0 - MultiThreaded (/MT)
                  # 1 - MultiThreadedDebug (/MTd)
                  # 2 - MultiThreadedDLL (/MD)
                  # 3 - MultiThreadedDebugDLL (/MDd)
                },
              },
            },
          }
        }],
      ]
    }
  ]
}