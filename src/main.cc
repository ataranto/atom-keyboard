#include "nan.h"
using namespace v8;

#include "keyboard.h"

namespace {

NAN_METHOD(Foo) {
  NanScope();
  printf("Foo");
  NanReturnValue(Integer::New(42));
}

void Init(Handle<Object> exports) {
  printf("Init");
  NODE_SET_METHOD(exports, "keyboard", Foo);
}

}  // namespace

NODE_MODULE(keyboard, Init)
