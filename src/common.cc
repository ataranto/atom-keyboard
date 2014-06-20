#include "common.h"

static uv_async_t g_async;
static uv_sem_t g_semaphore;
static uv_thread_t g_thread;

static KEY_TYPE g_key;
static Persistent<Function> g_callback;

static void CommonThread(void* handle) {
  WaitForMainThread();
}

static void MakeCallbackInMainThread(uv_async_t* handle, int status) {
  NanScope();

  if (!g_callback.IsEmpty()) {
    Handle<String> key;
    printf("MakeCallbackInMainThread, g_key: %d\n", g_key);
    switch (g_key) {
      case KEY_PLAYPAUSE:
        key = String::New("media-play-pause");
        break;
      case KEY_PREVIOUS:
        key = String::New("media-previous-track");
        break;
      case KEY_NEXT:
        key = String::New("media-next-track");
        break;
      default:
        fprintf(stderr, "Got unknown key: %d\n", g_key);
        return;
    }

    Handle<Value> argv[] = {
        key,
    };
    NanPersistentToLocal(g_callback)->Call(
        Context::GetCurrent()->Global(), 1, argv);
  }

  WakeupNewThread();
}

void CommonInit() {
  uv_sem_init(&g_semaphore, 0);
  uv_async_init(uv_default_loop(), &g_async, MakeCallbackInMainThread);
  uv_thread_create(&g_thread, &CommonThread, NULL);
}

void WaitForMainThread() {
  uv_sem_wait(&g_semaphore);
}

void WakeupNewThread() {
  printf("WakeupNewThread before\n");
  uv_sem_post(&g_semaphore);
  printf("WakeupNewThread after\n");
}

void PostKeyAndWait(KEY_TYPE key)
{
  g_key = key;
  printf("PostKeyAndWait: %d\n", key);

  uv_async_send(&g_async);
  WaitForMainThread();
}

NAN_METHOD(SetCallback) {
  NanScope();

  if (!args[0]->IsFunction())
    return NanThrowTypeError("Function required");

  NanAssignPersistent(Function, g_callback, Handle<Function>::Cast(args[0]));
  NanReturnUndefined();
}
