## 使用

`Image.data` - 為 RGBA 方式排列的圖像資料，array 總長度為 width * height * 4

參考： https://developer.mozilla.org/en-US/docs/Web/API/ImageData/data

`Image.hints` - 用於限縮搜尋條碼的範圍，字串 `"[QRCode]"` 代表只搜尋QRCode，`"[QRCode,PDF417]"` 代表搜尋 QRCode，PDF417 兩種條碼，或是不指定此參數，代表搜尋所有可能的條碼，這樣會花較多時間。

`Results` - 圖像中所有條碼的資訊

```js
const BarCodes = require("bindings")("BarCodes.node");

const Image = {
  data:[0,0,0,255,0,0,0,255...], // RGBA Data
  width:1280,
  height:720,
  hints:"[QRCode]" 
}

const Results = BarCodes.Read(Image)
const Result = Results[0];
/* BarCode Data
{
    "text": "1",
    "format": "QRCode",
    "contentType": "Text",
    "position": [
        {
            "x": 205,
            "y": 256
        },
        {
            "x": 252,
            "y": 175
        },
        {
            "x": 334,
            "y": 224
        },
        {
            "x": 285,
            "y": 304
        }
    ],
    "orientation": -59,
    "mirrored": false
}
*/
```

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


