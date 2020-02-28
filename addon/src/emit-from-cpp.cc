#include <node.h>
#include <libsimu.hpp>

#include <chrono>
#include <iostream>
#include <thread>

namespace groupsimulator {


using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

// This is the implementation of the "add" method
// Input arguments are passed using the
// const FunctionCallbackInfo<Value>& args struct
void Simu(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() < 3) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,
                            "Wrong number of arguments",
                            NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  // Check the argument types
  if (!args[0]->IsString() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate,
                            "Wrong arguments",
                            NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  libsimu::Time Result;
  String::Utf8Value Str(isolate, args[0]);
  std::string EventId(*Str);
  unsigned GroupSize  = args[1].As<Number>()->Value();
  unsigned Avg  = args[2].As<Number>()->Value();
  std::vector<libsimu::Time> Times(GroupSize, Avg);

  libsimu::SimuGroup(&Result, EventId, Times);


  // Set the return value (using the passed in
  // FunctionCallbackInfo<Value>&)
  args.GetReturnValue().Set((unsigned)Result);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "RunGroup", Simu);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)

}
