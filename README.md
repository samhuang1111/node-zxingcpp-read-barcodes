## 編譯 C++ Addon

準備 node-gyp 用來編譯 Addon

https://github.com/nodejs/node-gyp#installation

一個 Node C++ Addons 範例，有三種方式，這個 Addon 使用 node-addon-api(Node-API)

https://github.com/nodejs/node-addon-examples/tree/main/1_hello_world

關於 node-addon-api(Node-API)

https://github.com/nodejs/node-addon-api#node-addon-api-module

需要先把 Zxing-Cpp 編譯好才可以編譯 Addon

```js
npm run build //node-gyp configure && node-gyp build
```

編譯完之後會產生 build 資料夾

## 編譯 ZXing-cpp

ZXing-cpp 的文件

https://github.com/zxing-cpp/zxing-cpp#build-instructions

```
git clone https://github.com/zxing-cpp/zxing-cpp.git --single-branch --depth 1
cmake -S zxing-cpp -B zxing-cpp.release -DCMAKE_BUILD_TYPE=Release
cmake --build zxing-cpp.release --config release -j8
```

目錄下有 zxing-cpp、zxing-cpp.release 資料夾

zxing-cpp.release/core/Release 底下會有 ZXing.lib，編譯 Addon 需要用到。

## 使用 Addon

```js
const BarCodes = require("bindings")("BarCodes.node");

const Image = {
  data:[0,0,0,255,0,0,0,255...], // RGBA Format Data
  width:1280,
  height:720,
  hints:"[QRCode]" 
}

const Results = BarCodes.Read(Image)

```
