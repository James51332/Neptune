#include "neptunepch.h"
#include "MacApplication.h"

#import <Cocoa/Cocoa.h>
#include <time.h>

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
  
  m_InitialTime = clock_gettime_nsec_np(CLOCK_MONOTONIC) / 1e9;
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

Float32 MacApplication::PollTime()
{
  return static_cast<Float32>(clock_gettime_nsec_np(CLOCK_MONOTONIC) / 1e9 - m_InitialTime);
}

} // namespace Neptune
