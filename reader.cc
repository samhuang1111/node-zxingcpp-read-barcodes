#include <napi.h>

#include "ReadBarcode.h"

typedef unsigned char uchar;

Napi::Object readBarcodeFromImageView(Napi::Env &env, ZXing::ImageView iv, bool tryHarder, const std::string &format)
{
  using namespace ZXing;

  Napi::Object Result = Napi::Object::New(env);

  try
  {
    DecodeHints hints;
    hints.setTryHarder(tryHarder);
    hints.setTryRotate(tryHarder);
    hints.setTryInvert(tryHarder);
    hints.setTryDownscale(tryHarder);
    hints.setFormats(BarcodeFormatsFromString(format));

    ZXing::Results ZXingResults = ReadBarcodes(iv, hints);
    Napi::Array ReadResults = Napi::Array::New(env, ZXingResults.size());

    for (size_t i = 0; i < ZXingResults.size(); i++)
    {
      ZXing::Result ZxingResult = ZXingResults[i];

      Napi::Array location = Napi::Array::New(env, ZxingResult.position().size());
      for (size_t j = 0; j < ZxingResult.position().size(); j++)
      {
        Napi::Object point = Napi::Object::New(env);
        point.Set("x", ZxingResult.position()[j].x);
        point.Set("y", ZxingResult.position()[j].y);
        location[j] = point;
      }

      Napi::Object position = Napi::Object::New(env);
      Napi::Object topLeft = Napi::Object::New(env);
      Napi::Object topRight = Napi::Object::New(env);
      Napi::Object bottomLeft = Napi::Object::New(env);
      Napi::Object bottomRight = Napi::Object::New(env);
      topLeft.Set("x", ZxingResult.position().topLeft().x);
      topLeft.Set("y", ZxingResult.position().topLeft().y);
      topRight.Set("x", ZxingResult.position().topRight().x);
      topRight.Set("y", ZxingResult.position().topRight().y);
      bottomLeft.Set("x", ZxingResult.position().bottomLeft().x);
      bottomLeft.Set("y", ZxingResult.position().bottomLeft().y);
      bottomRight.Set("x", ZxingResult.position().bottomRight().x);
      bottomRight.Set("y", ZxingResult.position().bottomRight().y);
      position.Set("topLeft", topLeft);
      position.Set("topRight", topRight);
      position.Set("bottomLeft", bottomLeft);
      position.Set("bottomRight", bottomRight);


      // array element
      Napi::Object jsResult = Napi::Object::New(env);
      jsResult.Set("format", ToString(ZxingResult.format()));
      jsResult.Set("contentType", ToString(ZxingResult.contentType()));
      jsResult.Set("text", ZxingResult.text());
      jsResult.Set("orientation", ZxingResult.orientation());
      jsResult.Set("isMirrored", ZxingResult.isMirrored());
      jsResult.Set("position", position);
      jsResult.Set("location", location);

      ReadResults[i] = jsResult;
    }
    Result.Set("results", ReadResults);
    Result.Set("error", "");
  }
  catch (const std::exception &e)
  {
    Result.Set("results", Napi::Array::New(env));
    Result.Set("error", Napi::String::New(env, e.what()));
  }
  catch (...)
  {
    Result.Set("results", Napi::Array::New(env));
    Result.Set("error", Napi::String::New(env, "Unknown error"));
  }

  return Result;
}

Napi::Object Read(const Napi::CallbackInfo &info)
{
  using namespace ZXing;

  Napi::ArrayBuffer imgBuffer = info[0].As<Napi::ArrayBuffer>();
  Napi::Number imgWidth = info[1].As<Napi::Number>();
  Napi::Number imgHeight = info[2].As<Napi::Number>();
  // Napi::Boolean tryHarder = info[3].As<Napi::Boolean>();
  // Napi::String format = info[4].As<Napi::String>();

  bool tryHarder = static_cast<bool>(info[3].As<Napi::Boolean>());
  std::string format = static_cast<std::string>(info[4].As<Napi::String>());

  return readBarcodeFromImageView(info.Env(), {reinterpret_cast<uint8_t *>(imgBuffer.Data()), imgWidth, imgHeight, ImageFormat::RGBX}, tryHarder, format);
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  exports.Set(Napi::String::New(env, "Read"), Napi::Function::New(env, Read));
  return exports;
}

NODE_API_MODULE(addon, Init)