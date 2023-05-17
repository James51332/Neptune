#include "neptunepch.h"
#include "MacWindow.h"

#include "macos/MacKeyCode.h"
#include "macos/MacMouseCode.h"
#include "macos/MacRenderContext.h"

#include "core/KeyCode.h"
#include "core/MouseCode.h"
#include "core/Application.h"
#include "core/Event.h"
#include "core/WindowEvent.h"
#include "core/MouseEvent.h"
#include "core/KeyEvent.h"

#include "metal/MetalRenderDevice.h"

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

// ----- NeptuneWindowDelegate ---------

@interface NeptuneWindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation NeptuneWindowDelegate

- (BOOL)windowShouldClose:(NSWindow *)sender
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::WindowClosedEvent>());
  return NO;
}

- (void)windowDidResize:(NSNotification *)notification
{
  NSWindow* window = [notification object];
  NSRect frame = [window contentRectForFrameRect: window.frame];
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::WindowResizedEvent>((UInt32)frame.size.width, (UInt32)frame.size.height));
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification
{
  NSWindow* window = [notification object];
  NSRect frame = [window contentRectForFrameRect: window.frame];
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::WindowResizedEvent>((UInt32)frame.size.width, (UInt32)frame.size.height));
}

- (void)windowWillExitFullScreen:(NSNotification *)notification
{
  NSWindow* window = [notification object];
  NSRect frame = [window contentRectForFrameRect: window.frame];
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::WindowResizedEvent>((UInt32)frame.size.width, (UInt32)frame.size.height));
}

@end

// ----- NeptuneView ---------

@interface NeptuneView : NSView <NSTextInputClient>
{
  Neptune::MacWindow* windowHandle;
  NSAttributedString* string;
  
  UInt64 modifierStates;
  UInt64 usedModifiers; // Mask to 0 all unimportant modifier flags
}

- (id) initWithWindow: (Neptune::MacWindow*) window;
@end

static const NSRange emptyRange = NSMakeRange(NSNotFound, 0);
@implementation NeptuneView

- (id) initWithWindow: (Neptune::MacWindow*) window
{
  if (self = [super init])
  {
  	windowHandle = window;
  	
  	modifierStates = 0;
  	usedModifiers = NSEventModifierFlagOption
  	| NSEventModifierFlagShift
  	| NSEventModifierFlagCommand
  	| NSEventModifierFlagCapsLock
  	| NSEventModifierFlagControl;
	}
  return self;
}

- (BOOL) canBecomeKeyView
{
  return YES;
}

- (BOOL) wantsUpdateLayer
{
  return YES;
}

- (BOOL) wantsLayer
{
  return YES;
}

- (void)updateLayer
{
}

- (BOOL) acceptsFirstResponder
{
  return YES;
}

- (BOOL) hasMarkedText
{
  return NO;
}

- (NSRange)markedRange
{
  return emptyRange;
}

- (NSRange)selectedRange
{
  return emptyRange;
}

- (void) setMarkedText: (nonnull id)string
         selectedRange: (NSRange)selectedRange
      replacementRange: (NSRange)replacementRange
{
}

- (void) unmarkText
{
}

- (NSArray*) validAttributesForMarkedText
{
  return [NSArray array];
}

- (NSAttributedString *) attributedSubstringForProposedRange: (NSRange)range
                                                 actualRange: (nullable NSRangePointer)actualRange
{
  return nil;
}

- (void) insertText: (id)string
   replacementRange: (NSRange)replacementRange
{
  NSString* chars;
  
  if ([string isKindOfClass: [NSAttributedString class]])
    chars = [string string];
  else
    chars = (NSString*) string;
  
  NSRange range = { 0, [chars length] };
  while (range.length)
  {
    UInt32 codepoint = 0;
    
    if ([chars getBytes: &codepoint
              maxLength: sizeof(codepoint)
             usedLength: NULL
               encoding: NSUTF32StringEncoding
                options: 0
                  range: range
         remainingRange: &range])
    {
      if (codepoint >= 0xf700 && codepoint <= 0xf7ff)
        continue;
      
      Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::KeyTypedEvent>(codepoint));
    }
  }
}

- (NSUInteger)characterIndexForPoint: (NSPoint)point
{
  return 0;
}

- (NSRect)firstRectForCharacterRange: (NSRange)range
                         actualRange: (nullable NSRangePointer)actualRange
{
  return NSMakeRect(0.0, 0.0, 0.0, 0.0);
}

- (void)doCommandBySelector: (nonnull SEL)selector
{
}

- (void)mouseDown:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MousePressedEvent>(Neptune::MouseLeft));
}

- (void)mouseDragged:(NSEvent *)event
{
  [self mouseMoved: event];
}

- (void)mouseUp:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MouseReleasedEvent>(Neptune::MouseLeft));
}

- (void)mouseMoved:(NSEvent *)event
{
  NSPoint location = [event locationInWindow];
  NSRect windowSize = [[event window] contentRectForFrameRect: [[event window] frame]];
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MouseMovedEvent>(location.x, windowSize.size.height - location.y));
}

- (void)rightMouseDown:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MousePressedEvent>(Neptune::MouseRight));
}

- (void)rightMouseDragged:(NSEvent *)event
{
  [self mouseMoved: event];
}

- (void)rightMouseUp:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MouseReleasedEvent>(Neptune::MouseRight));
}

- (void)otherMouseDown:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MousePressedEvent>(Neptune::s_MouseTranslations[[event buttonNumber]]));
}

- (void)otherMouseDragged:(NSEvent *)event
{
  [self mouseMoved: event];
}

- (void)otherMouseUp:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MouseReleasedEvent>(Neptune::s_MouseTranslations[[event buttonNumber]]));
}

- (void)scrollWheel:(NSEvent *)event
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::MouseScrolledEvent>([event scrollingDeltaY], [event scrollingDeltaX]));
}

- (void)keyDown:(NSEvent *)event
{
  Neptune::KeyCode key = Neptune::s_KeyTranslations[[event keyCode]];
  if (key != Neptune::KeyNone)
    Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::KeyPressedEvent>(key));
  [self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent *)event
{
  Neptune::KeyCode key = Neptune::s_KeyTranslations[[event keyCode]];
  if (key != Neptune::KeyNone)
    Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::KeyReleasedEvent>(key));
}

- (void)flagsChanged:(NSEvent *)event
{
  unsigned long changed = ([event modifierFlags] ^ modifierStates) & usedModifiers;
  
  // We can't distinguish left and right modifiers. All will be processed as lefts.
  Neptune::KeyCode key;
  switch (changed)
  {
    case NSEventModifierFlagOption: key = Neptune::KeyLeftAlt; break;
    case NSEventModifierFlagShift: key = Neptune::KeyLeftShift; break;
    case NSEventModifierFlagCommand: key = Neptune::KeyLeftSuper; break;
    case NSEventModifierFlagCapsLock: key = Neptune::KeyCapsLock; break;
    case NSEventModifierFlagControl: key = Neptune::KeyLeftControl; break;
    default: return; // We don't care about any others
  }
  
  if (changed & modifierStates) // Key released (difference is on a bit that was 1)
    Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::KeyReleasedEvent>(key));
  else
    Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::KeyPressedEvent>(key));
  
  modifierStates = [event modifierFlags];
}
@end

// ----- MacWindow ---------

namespace Neptune
{

MacWindow::MacWindow(const WindowDesc& desc)
	: Window(desc), m_Window(nullptr), m_WindowDelegate(nullptr)
{
  NSRect content = NSMakeRect(0.0, 0.0, desc.Width, desc.Height);
  NSInteger styleMask = NSWindowStyleMaskClosable
  										| NSWindowStyleMaskMiniaturizable
  										| NSWindowStyleMaskTitled;

  
  if (desc.Resizeable)
    styleMask |= NSWindowStyleMaskResizable;
  
  NSWindow* window = [[NSWindow alloc] initWithContentRect: content
                                                 styleMask: styleMask
                                                   backing: NSBackingStoreBuffered
                                                     defer: NO];
  
  NEPTUNE_ASSERT(window, "Failed to create window!");
  
  if (desc.Name.Raw())
    [window setTitle:@(desc.Name.Raw())];
  [window center];
  
  m_WindowDelegate = [[NeptuneWindowDelegate alloc] init];
  NEPTUNE_ASSERT(m_WindowDelegate, "Failed to create window delegate!");
  
  [window setDelegate: (NeptuneWindowDelegate*)m_WindowDelegate];
  
  m_View = [[NeptuneView alloc] initWithWindow: this];
  NEPTUNE_ASSERT(m_View, "Failed to create view!");
  
  [window setAcceptsMouseMovedEvents: YES];
  [window setContentView: (NeptuneView*)m_View];
  [window makeFirstResponder: (NeptuneView*)m_View];
  
  m_Window = window;
}

MacWindow::~MacWindow()
{
  [(NSWindow*)m_Window setContentView: nil];
  [(NeptuneView*)m_View release];
  m_View = nullptr;
  
  [(NSWindow*)m_Window setDelegate: nil];
  [(NeptuneWindowDelegate*)m_WindowDelegate release];
  m_WindowDelegate = nullptr;
  
  [(NSWindow*)m_Window close];
  [(NSWindow*)m_Window release];
  m_Window = nullptr;
}

void MacWindow::SetDesc(const WindowDesc& desc)
{
  m_Desc = desc;
  NSWindow* window = (NSWindow*)m_Window;
  
  if (desc.Resizeable)
    [window setStyleMask: [window styleMask] | NSWindowStyleMaskResizable];
  
  if (desc.Name.Raw())
    [window setTitle:@(desc.Name.Raw())];
  
  [window setContentSize: NSMakeSize(desc.Width, desc.Height)];
}

void MacWindow::SetContext(const Ref<RenderContext>& ctx)
{
  RenderAPI api = ctx->GetAPI();
  switch (api) {
    case RenderAPI::None: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); return; }
    case RenderAPI::OpenGL: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); return; }
    case RenderAPI::Metal: {
      Ref<MacMetalRenderContext> context = StaticRefCast<MacMetalRenderContext>(ctx);
      ((NeptuneView*)m_View).layer = (CAMetalLayer*)context->GetLayer();
      break;
    }
    case RenderAPI::Vulkan: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); return; }
    case RenderAPI::DirectX: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); return; }
  }
}

void MacWindow::Show()
{
  [(NSWindow*)m_Window makeKeyAndOrderFront: nil];
  [(NSWindow*)m_Window makeMainWindow];
}

void MacWindow::Hide()
{
  [(NSWindow*)m_Window orderOut: (NSWindow*)m_Window];
}

void MacWindow::Close()
{
  [(NSWindow*)m_Window close];
}

} // namespace Neptune
