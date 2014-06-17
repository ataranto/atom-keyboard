#include <nan.h>
using namespace v8;

namespace {

NAN_METHOD(Foo) {
  printf("Foo()\n");
  NanScope();

  NanReturnValue(NanNew<Number>(2));
}

void Init(Handle<Object> exports) {
  printf("Init()\n");
  exports->Set(NanSymbol("foo"),
    NanNew<FunctionTemplate>(Foo)->GetFunction());
}

}  // namespace

NODE_MODULE(keyboard, Init)
