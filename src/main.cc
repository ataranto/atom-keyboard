#include <nan.h>
using namespace v8;

namespace {

NAN_METHOD(Foo) {
  printf("Foo()\n");
  NanScope();

  NanReturnValue(2);
}

void Init(Handle<Object> exports) {
  printf("Init()\n");

  NODE_SET_METHOD(exports, "foo", Foo);
}

}  // namespace

NODE_MODULE(keyboard, Init)
