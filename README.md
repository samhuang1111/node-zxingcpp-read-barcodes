





## 編譯Node-Addon
參考：

https://github.com/nodejs/node-addon-api#setup

https://github.com/nodejs/node-addon-examples/tree/HEAD/1_hello_world/node-addon-api


## 編譯ZXing-cpp

參考：
https://github.com/zxing-cpp/zxing-cpp#build-instructions

安裝 CMake
安裝 C++17 編譯器 (minimum VS 2019 16.8 / gcc 7 / clang 5).

```
git clone https://github.com/zxing-cpp/zxing-cpp.git --single-branch --depth 1
cmake -S zxing-cpp -B zxing-cpp.release -DCMAKE_BUILD_TYPE=Release
cmake --build zxing-cpp.release -j8
```