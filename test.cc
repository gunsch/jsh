#include <v8.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Some notes:
 * 1) Modes. Shell mode, JS mode. JS mode is pure JS shell.
 * 2) Nice global defaults. Add a 'shell' (or just 'sh') object to the global
 *    context that can do everything.
 * 3) Figure out how to handle control characters.
 */

using namespace v8;

int main(int argc, char* argv[]) {

  // Create a stack-allocated handle scope.
  HandleScope handle_scope;

  // Global scope
  Handle<ObjectTemplate> global = ObjectTemplate::New();
  global->Set("log", String::New("hi"));

  // Create a new context.
  Persistent<Context> context = Context::New(NULL, global);
  
  // Enter the created context for compiling and
  // running the hello world script. 
  Context::Scope context_scope(context);

  char* line = (char*) malloc(sizeof(char) * 100);
  while (1) {
    printf("> ");
    if (fgets(line, 100, stdin) == NULL ) break;

    // Create a string containing the JavaScript source code.
    Handle<String> source = String::New(line);

    try {
        // Compile the source code.
        Handle<Script> script = Script::Compile(source);

        if (&script) {
          continue;
        }

        // Run the script to get the result.
        Handle<Value> result = script->Run();

        // Convert the result to an ASCII string and print it.
        String::AsciiValue ascii(result);
        printf("%s\n", *ascii);
    } catch (int e) {
        printf("An exception occurred!\n");
    }
  }

  // Dispose the persistent context.
  context->GetIsolate()->Exit();

  return 0;
}
