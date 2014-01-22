#include <v8.h>

#ifndef ENVIRONMENT_H_GUARD
#define ENVIRONMENT_H_GUARD

namespace jsh {

class Environment {
public:
  static v8::Handle<v8::ObjectTemplate> CreateEnvironment(v8::Isolate* isolate);
private:
  static void EnvironmentGetter(v8::Local<v8::String> property,
      const v8::PropertyCallbackInfo<v8::Value>& info);
  static void EnvironmentSetter(v8::Local<v8::String> property,
      v8::Local<v8::Value> value,
      const v8::PropertyCallbackInfo<v8::Value>& info);
  static void EnvironmentDeleter(v8::Local<v8::String> property,
      const v8::PropertyCallbackInfo<v8::Boolean>& info);
  static void EnvironmentEnumerator(
    const v8::PropertyCallbackInfo<v8::Array>& info);
};

}

#endif
