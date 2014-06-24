#include "common.h"

static KEY_TYPE g_key;
static uv_rwlock_t g_rwlock;
static uv_async_t g_async;
static Persistent<Function> g_callback;

static void Callback(uv_async_t* handle, int status) {
  NanScope();

  if (g_callback.IsEmpty()) {
    return;
  }

  uv_rwlock_rdlock(&g_rwlock);

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

  uv_rwlock_rdunlock(&g_rwlock);

  Handle<Value> argv[] = { key };
  NanPersistentToLocal(g_callback)->Call(
    Context::GetCurrent()->Global(), 1, argv);
}

void CommonInit() {
  uv_rwlock_init(&g_rwlock);
  uv_async_init(uv_default_loop(), &g_async, Callback);
}

void PostKey(KEY_TYPE key) {
  uv_rwlock_wrlock(&g_rwlock);

  // XXX: it may be preferable to use uv_async_t.data to pass the key parameter,
  // rather than stashing it in the global g_key. see:
  //
  // http://nikhilm.github.io/uvbook/threads.html
  // https://github.com/thlorenz/libuv-dox/blob/master/examples/13-progress/main.c
  g_key = key;
  uv_async_send(&g_async);

  uv_rwlock_wrunlock(&g_rwlock);
}

NAN_METHOD(SetCallback) {
  NanScope();

  if (!args[0]->IsFunction())
    return NanThrowTypeError("Function required");

  NanAssignPersistent(Function, g_callback, Handle<Function>::Cast(args[0]));
  NanReturnUndefined();
}
