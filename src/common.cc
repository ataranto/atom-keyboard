#include "common.h"

static uv_async_t g_async;
static uv_sem_t g_semaphore;
static Persistent<Function> g_callback;

static void MakeCallbackInMainThread(uv_async_t* handle, int status) {
  NanScope();

  if (!g_callback.IsEmpty()) {
    KEY_TYPE key_type = *(KEY_TYPE *)handle->data;
    printf("MakeCallbackInMainThread key_type: %d\n", key_type);

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

  uv_sem_post(&g_semaphore);
}

void CommonInit() {
  uv_sem_init(&g_semaphore, 0);
  uv_async_init(uv_default_loop(), &g_async, MakeCallbackInMainThread);

  uv_sem_post(&g_semaphore);
}

void PostKey(KEY_TYPE key)
{
  printf("PostKey: %d\n", key);

  g_async.data = (void *)&key;
  uv_async_send(&g_async);

  uv_sem_wait(&g_semaphore);
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
