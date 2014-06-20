#include "common.h"

static HHOOK g_hhook;

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam) {
  printf("code: %d, wParam: %d, lParam: %d\n", code, wParam, lParam);

  tagKBDLLHOOKSTRUCT *s = (tagKBDLLHOOKSTRUCT *)lParam;
  printf("vkCode: %d\n", s->vkCode);

  if (wParam == WM_KEYDOWN) {
    printf("WM_KEYDOWN\n");
  }

  return CallNextHookEx(g_hhook, code, wParam, lParam);
}

void PlatformInit() {
  HMODULE hModule = GetModuleHandle(0);
  g_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hModule, NULL);
  
  WakeupNewThread();
}
