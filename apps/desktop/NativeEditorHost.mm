#include "NativeEditorHost.hpp"

#import <Cocoa/Cocoa.h>
void* sys01OpenEditorParent() {
    [NSApplication sharedApplication];
    auto* window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 640, 440)
                                               styleMask:NSWindowStyleMaskTitled
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
    [window setReleasedWhenClosed:NO];
    [window orderFront:nil];
    return window;
}
void sys01PumpEditor() {
    @autoreleasepool {
        while (auto* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantPast]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES])
            [NSApp sendEvent:event];
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate date]];
    }
}
void sys01CloseEditorParent(void* p) {
    auto* window = static_cast<NSWindow*>(p);
    [window close];
    [window release];
}
// NSView is the VST3 kPlatformTypeNSView parent.
extern "C" void* sys01EditorView(void* p) {
    return [static_cast<NSWindow*>(p) contentView];
}
