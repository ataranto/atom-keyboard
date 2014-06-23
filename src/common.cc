#include "common.h"

static KEY_TYPE g_key;
static uv_rwlock_t g_lock;
static uv_async_t g_async;
static Persistent<Function> g_callback;

static void MakeCallbackInMainThread(uv_async_t* handle, int status) {
  NanScope();

  if (!g_callback.IsEmpty()) {
    uv_rwlock_rdlock(&g_lock);
    printf("MakeCallbackInMainThread, g_key: %d\n", g_key);

    Handle<String> key;
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

    uv_rwlock_rdunlock(&g_lock);

    Handle<Value> argv[] = {
      key,
    };
    NanPersistentToLocal(g_callback)->Call(
        Context::GetCurrent()->Global(), 1, argv);
  } else {
    printf("g_callback.IsEmpty() == true\n");
  }
}

void CommonInit() {
  uv_rwlock_init(&g_lock);
  uv_async_init(uv_default_loop(), &g_async, MakeCallbackInMainThread);
}

void PostKey(KEY_TYPE key)
{
  printf("PostKey: %d\n", key);

  uv_rwlock_wrlock(&g_lock);

  // XXX: global
  g_key = key;
  uv_async_send(&g_async);

  uv_rwlock_wrunlock(&g_lock);
}

NAN_METHOD(SetCallback) {
  NanScope();

  if (!args[0]->IsFunction())
    return NanThrowTypeError("Function required");

  NanAssignPersistent(Function, g_callback, Handle<Function>::Cast(args[0]));

  // XXX immediately attempt to call callback
  Handle<String> arg = String::New("test");
  Handle<Value> argv[] = { arg, };
  NanPersistentToLocal(g_callback)->Call(
    Context::GetCurrent()->Global(), 1, argv);

  NanReturnUndefined();
}
