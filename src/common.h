#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include "nan.h"
using namespace v8;

void CommonInit();
void PlatformInit();

enum KEY_TYPE {
    KEY_NONE,
    KEY_PLAYPAUSE,
    KEY_PREVIOUS,
    KEY_NEXT,
};

void WaitForMainThread();
void WakeupNewThread();
void PostKey(KEY_TYPE key);

NAN_METHOD(SetCallback);

#endif  // SRC_COMMON_H_
