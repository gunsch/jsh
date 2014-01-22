#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <v8.h>

#include "environment.h"

// TODO Try to make a template that generates this from a string?
static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info) {
  info.GetReturnValue().Set(v8::String::NewFromUtf8(
      info.GetIsolate(), "[Environment]"));
}

void jsh::Environment::EnvironmentGetter(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::String::Utf8Value utf8PropertyName(property);

  if (strcmp(*utf8PropertyName, "toString") == 0) {
    info.GetReturnValue().Set(v8::Function::New(info.GetIsolate(), ToString));
    return;
  }

  char* envValue = getenv(*utf8PropertyName);
  if (envValue == NULL) {
    info.GetReturnValue().SetEmptyString();
    return;
  }

  info.GetReturnValue().Set(
      v8::String::NewFromUtf8(info.GetIsolate(), envValue));
}

void jsh::Environment::EnvironmentSetter(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
  v8::String::Utf8Value utf8PropertyName(property);
  v8::String::Utf8Value utf8PropertyValue(value->ToString());

  setenv(*utf8PropertyName, *utf8PropertyValue, 1);
  info.GetReturnValue().Set(value);
}

void jsh::Environment::EnvironmentDeleter(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Boolean>& info) {
  v8::String::Utf8Value utf8PropertyName(property);

  int result = unsetenv(*utf8PropertyName);
  info.GetReturnValue().Set(result == 0);
}

void jsh::Environment::EnvironmentEnumerator(
    const v8::PropertyCallbackInfo<v8::Array>& info) {
  v8::Handle<v8::Array> environmentVariableNames =
      v8::Array::New(info.GetIsolate(), 0);

  int i = 0;
  char** environIter = environ;
  for (int i = 0; *environIter != 0; environIter++, i++) {
    std::string envNameAndValue(*environIter);
    std::string envName = envNameAndValue.substr(0, envNameAndValue.find('='));
    environmentVariableNames->Set(i,
        v8::String::NewFromUtf8(info.GetIsolate(), envName.c_str()));
  }

  info.GetReturnValue().Set(environmentVariableNames);
}

v8::Handle<v8::ObjectTemplate>
jsh::Environment::CreateEnvironment(v8::Isolate* isolate) {
  v8::Handle<v8::ObjectTemplate> environment_template =
      v8::ObjectTemplate::New(isolate);

  environment_template->SetNamedPropertyHandler(
      EnvironmentGetter, EnvironmentSetter, 0, EnvironmentDeleter,
      EnvironmentEnumerator);

  return environment_template;
}
