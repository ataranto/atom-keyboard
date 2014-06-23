#include "common.h"

static uv_rwlock_t g_lock;
static uv_async_t g_async;
static Persistent<Function> g_callback;

static void MakeCallbackInMainThread(uv_async_t* handle, int status) {
  NanScope();

  if (!g_callback.IsEmpty()) {
    uv_rwlock_rdlock(&g_lock);
    KEY_TYPE key_type = *(KEY_TYPE *)handle->data;
    printf("MakeCallbackInMainThread key_type: %d\n", key_type);
    uv_rwlock_rdunlock(&g_lock);

    Handle<String> key;
    switch (key_type) {
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
        fprintf(stderr, "Got unknown key: %d\n", key_type);
        return;
    }

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

  void *data = (void *)&key;
  g_async.data = data;
  printf("data1: %d\n", *(KEY_TYPE *)data);
  printf("data2: %d\n", *(KEY_TYPE *)g_async.data);
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
