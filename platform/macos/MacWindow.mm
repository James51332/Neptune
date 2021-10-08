#include "neptunepch.h"
#include "MacWindow.h"

#include "core/WindowEvent.h"
#include "core/Application.h"

#include <Cocoa/Cocoa.h>

@interface NeptuneWindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation NeptuneWindowDelegate

- (BOOL)windowShouldClose:(NSWindow *)sender
{
  Neptune::Application::PushEvent(Neptune::CreateScope<Neptune::WindowClosedEvent>());
  return NO;
}

@end

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
  
  m_Window = window;
}

MacWindow::~MacWindow()
{
  [(NSWindow*)m_Window close];
  [(NSWindow*)m_Window release];
  m_Window = nullptr;
  
  [(NeptuneWindowDelegate*)m_WindowDelegate release];
  m_WindowDelegate = nullptr;
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
