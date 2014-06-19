#include "common.h"

void Init(Handle<Object> exports) {
  CommonInit();
  PlatformInit();

  NODE_SET_METHOD(exports, "setCallback", SetCallback);
}

NODE_MODULE(keyboard, Init)
