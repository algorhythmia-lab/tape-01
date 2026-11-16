#import <Cocoa/Cocoa.h>
bool sys01HasNativeCassetteWindow(void* nativeView) {
    auto* window = [static_cast<NSView*>(nativeView) window];
    return window != nil && (window.styleMask & NSWindowStyleMaskTitled) == 0 && !window.opaque &&
           window.miniaturizable;
}
