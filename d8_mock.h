#include <d8.h>
#include <v8.h>

#include <v8.h>

#ifndef D8_MOCK_H_GUARD
#define D8_MOCK_H_GUARD

namespace v8 {
  class ReadLineEditor: public LineEditor {
    public:
      ReadLineEditor() : LineEditor(LineEditor::READLINE, "readline") { }
      virtual v8::Handle<v8::String> Prompt(const char* prompt);
      virtual bool Open(Isolate* isolate);
      virtual bool Close();
      virtual void AddHistory(const char* str);
  };
}

// Dummy LineEditor class.
v8::LineEditor::LineEditor(v8::LineEditor::Type type, const char* name) {}

// Satisfy the GetCompletions API.
v8::Handle<v8::Array>
v8::Shell::GetCompletions(v8::Isolate* isolate, v8::Handle<v8::String> text,
    v8::Handle<v8::String> full) {
  // TODO Tab-complete segfaults right now.
  return v8::Array::New(isolate, 0);
}

#endif
