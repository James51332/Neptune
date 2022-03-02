#include "neptunepch.h"
#include "MacApplication.h"

#import <Cocoa/Cocoa.h>

@interface NeptuneDelegate : NSObject<NSApplicationDelegate>
@end

@implementation NeptuneDelegate

-(BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
  return NO;
}
@end

namespace Neptune
{

MacApplication::MacApplication()
{
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular]; // if we're unbundled, give resize handles
  
  m_AppDelegate = [[NeptuneDelegate alloc] init];
  [NSApp setDelegate: (NeptuneDelegate*)m_AppDelegate];
}

MacApplication::~MacApplication()
{
  [NSApp setDelegate:nil];
  [((NeptuneDelegate*)m_AppDelegate) release];
  
  [NSApp terminate: nil];
}

void MacApplication::PollEvents() noexcept
{
  for (;;)
  {
    NSEvent* event = [NSApp nextEventMatchingMask: NSEventMaskAny
                                        untilDate: nil
                                           inMode: NSDefaultRunLoopMode
                                          dequeue: YES];
    
    if (!event)
      break;
    else
      [NSApp sendEvent: event];
  }
}

} // namespace Neptune
