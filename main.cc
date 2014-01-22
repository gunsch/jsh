#include <d8.h>
#include <stdio.h>
#include <v8.h>

#include "d8_mock.h"
#include "environment.h"

// Executes a string within the current v8 context.
bool ExecuteString(v8::Isolate* isolate, v8::Handle<v8::String> source) {
  v8::TryCatch try_catch;
  v8::HandleScope handle_scope(isolate);
  v8::Handle<v8::Script> script = v8::Script::Compile(source);
  if (script.IsEmpty()) {
    v8::String::Utf8Value exception(try_catch.Exception());
    printf("Syntax Error: %s\n", *exception);
    return false;
  } else {
    v8::Handle<v8::Value> result = script->Run();
    if (result.IsEmpty()) {
      v8::String::Utf8Value exception(try_catch.Exception());
      printf("Runtime Error: %s\n", *exception);
      return false;
    } else {
      v8::Handle<v8::String> stringResult = result->ToString();
      if (stringResult.IsEmpty()) {
        printf("[no string representation]");
      } else {
        v8::String::Utf8Value str(stringResult);
        printf("%s\n", *str);
      }
      printf("\n");
      return true;
    }
  }
}

void RunShell(v8::Isolate* isolate) {
  v8::Locker locker(isolate);
  v8::HandleScope outer_scope(isolate);
  // Initiate ReadLineEditor
  v8::ReadLineEditor console;
  console.Open(isolate);
  while (true) {
    v8::HandleScope inner_scope(isolate);
    v8::Handle<v8::String> input = console.Prompt("> ");
    if (input.IsEmpty()) break;
    ExecuteString(isolate, input);
  }
  printf("\n");
}

int main(int argc, char* argv[]) {
  // Get the default v8::Isolate created at startup.
  v8::Isolate* isolate = v8::Isolate::GetCurrent();

  // Create a stack-allocated handle scope.
  v8::HandleScope handle_scope(isolate);

  v8::Handle<v8::ObjectTemplate> global_scope = v8::ObjectTemplate::New();
  v8::Shell::AddOSMethods(isolate, global_scope);
  global_scope->Set(v8::String::NewFromUtf8(isolate, "env"), 
      jsh::Environment::CreateEnvironment(isolate));

  v8::Handle<v8::Context> context =
      v8::Context::New(isolate, NULL, global_scope);
  v8::Context::Scope context_scope(context);

  RunShell(isolate);

  return 0;
}
