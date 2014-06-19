#include "common.h"

static HHOOK g_hhook;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
  printf("code: %d\n", code);

  return CallNextHookEx(g_hhook, code, wParam, lParam);
}

void PlatformInit() {
  HMODULE hModule = GetModuleHandle(0);
  g_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hModule, NULL);
  
  WakeupNewThread();
}
