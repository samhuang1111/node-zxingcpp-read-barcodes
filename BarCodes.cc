#include <napi.h>
#include <vector>
#include <codecvt>
#include <iostream>
#include "ReadBarcode.h"

using namespace std;
typedef unsigned char uchar;

std::string wsUtf16ToUtf8(const std::wstring &utf16Str);

Napi::Value ResultsToJSResults(Napi::Env &env, ZXing::Results &decodeResults);

ZXing::ImageView JSImageDataToImageView(Napi::Value &infoData, Napi::Value &infoWidth, Napi::Value &infoHeight);

ZXing::Results ReadBarCodes(const Napi::Value &paramValue);

inline std::string wsUtf16ToUtf8(const std::wstring &utf16Str)
{
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
  return conv.to_bytes(utf16Str);
}

// Convert ZXing::Results To Napi::Value(Javascript Object)
inline Napi::Value ResultsToJSResults(Napi::Env &env, ZXing::Results &decodeResults)
{

  Napi::Array jsResults = Napi::Array::New(env, decodeResults.size()); // js array

  // Format C++ ZXing::Results to Javascript Object
  for (size_t i = 0; i < decodeResults.size(); i++)
  {

    ZXing::Result result = decodeResults[i];

    auto resText = result.text();             // decode text
    auto resFormat = ZXing::ToString(result.format());       // decode format
    auto resContent = ZXing::ToString(result.contentType()); // deocde content type
    auto resRotation = result.orientation();                   // decode rotate
    auto resPosition = result.position();                    // decode position
    auto resMirror = result.isMirrored();

    Napi::Object item = Napi::Object::New(env); // js object

    auto text = Napi::String::New(env, resText);
    auto format = Napi::String::New(env, resFormat);
    auto content = Napi::String::New(env, resContent);
    auto rotation = Napi::Number::New(env, resRotation);
    auto position = Napi::Array::New(env, resPosition.size());
    auto mirrored = Napi::Boolean::New(env, resMirror);

    for (int i = 0; i < resPosition.size(); i++)
    {
      Napi::Object point = Napi::Object::New(env);
      auto x = resPosition[i].x;
      auto y = resPosition[i].y;
      point.Set("x", x);
      point.Set("y", y);
      position[i] = point;
    }

    item.Set("text", text); // set key value to object
    item.Set("format", format);
    item.Set("contentType", content);
    item.Set("position", position);
    item.Set("orientation", rotation);
    item.Set("mirrored",mirrored);

    jsResults[i] = item;
  }

  return jsResults;
}

// Convert Javascript ImageData To ZXing::ImageView
inline ZXing::ImageView JSImageDataToImageView(Napi::Value &infoData, Napi::Value &infoWidth, Napi::Value &infoHeight)
{

  Napi::TypedArray srcTypedArray = infoData.As<Napi::TypedArray>();      // js image typeArray data
  Napi::Uint8Array srcUint8Array = srcTypedArray.As<Napi::Uint8Array>(); // js image uint8Array data

  std::vector<uchar> srcVector(srcUint8Array.Data(), srcUint8Array.Data() + srcUint8Array.ElementLength());

  auto width = infoWidth.As<Napi::Number>().Uint32Value();   // image width
  auto height = infoHeight.As<Napi::Number>().Uint32Value(); // image height
  auto format = ZXing::ImageFormat::RGBX;                    // image channels

  uchar *srcBuffer = new uchar[srcVector.size()];
  std::copy(srcVector.begin(), srcVector.end(), srcBuffer);

  ZXing::ImageView srcView = ZXing::ImageView(srcBuffer, width, height, format);

  srcVector.clear();
  return srcView;
}

// Read BarCodes
inline ZXing::Results ReadBarCodes(const Napi::Value &imageInfo)
{
  Napi::Object imageParam = imageInfo.As<Napi::Object>();

  Napi::Value infoData = imageParam.Get("data");
  Napi::Value infoWidth = imageParam.Get("width");
  Napi::Value infoHeight = imageParam.Get("height");
  Napi::Value infoHints = imageParam.Get("hints");

  ZXing::DecodeHints decodeHints{};

  if (infoHints.IsString())
  {
    std::string strHints = infoHints.As<Napi::String>().Utf8Value();
    decodeHints.setFormats(ZXing::BarcodeFormatsFromString(strHints));
  }

  ZXing::ImageView decodeImageView = JSImageDataToImageView(infoData, infoWidth, infoHeight);
  ZXing::Results decodeResults = ZXing::ReadBarcodes(decodeImageView, decodeHints);

  delete[] decodeImageView.data(0, 0);

  return decodeResults;
}

// Read
Napi::Value Read(const Napi::CallbackInfo &info)
{
  if (!info.Length())
  {
    Napi::TypeError::New(info.Env(), "Read() Parameter is Null!").ThrowAsJavaScriptException();
  }
  if (!info[0].IsObject())
  {
    Napi::TypeError::New(info.Env(), "Read() Parameter is not Object!").ThrowAsJavaScriptException();
  }

  ZXing::Results decodeResults = ReadBarCodes(info[0]);

  if (!decodeResults.size())
  {
    return info.Env().Null();
  }

  return ResultsToJSResults(info.Env(), decodeResults);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "Read"), Napi::Function::New(env, Read));
  return exports;
}

NODE_API_MODULE(addon, Init)