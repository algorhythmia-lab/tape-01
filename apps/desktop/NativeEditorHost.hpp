#pragma once
// Minimal macOS host surface for loaded-binary fixtures, never shipping code.
void* sys01OpenEditorParent();
void sys01PumpEditor();
void sys01CloseEditorParent(void*);

extern "C" void* sys01EditorView(void*);
