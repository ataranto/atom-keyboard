#include "common.h"

static HHOOK g_hhook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  KBDLLHOOKSTRUCT *s = (KBDLLHOOKSTRUCT *)lParam;
  LRESULT result = CallNextHookEx(g_hhook, nCode, wParam, lParam);

  if (wParam == WM_KEYDOWN) {
    KEY_TYPE key = KEY_NONE;
    // Virtual-Key Codes
    // http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
    switch (s->vkCode) {
      case VK_MEDIA_PLAY_PAUSE:
      case VK_F8:
        key = KEY_PLAYPAUSE;
        break;
      case VK_MEDIA_PREV_TRACK:
      case VK_F7:
        key = KEY_PREVIOUS;
        break;
      case VK_MEDIA_NEXT_TRACK:
      case VK_F9:
        key = KEY_NEXT;
        break;
    }

    if (key != KEY_NONE) {
      PostKey(key);
    }
  }

  return result;
}

void PlatformInit() {
  HMODULE hModule = GetModuleHandle(0);
  g_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hModule, NULL);
}
