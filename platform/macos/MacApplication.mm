#include "neptunepch.h"
#include "MacApplication.h"

#include <Cocoa/Cocoa.h>

namespace Neptune
{

MacApplication::MacApplication()
{
  [NSApplication sharedApplication];
  [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
}

MacApplication::~MacApplication()
{
  [NSApp terminate: nil];
}

void MacApplication::PollEvents() noexcept
{
  for (;;)
  {
    NSEvent* event = [NSApp nextEventMatchingMask: NSEventMaskAny
                                        untilDate: [NSDate distantPast]
                                           inMode: NSDefaultRunLoopMode
                                          dequeue: YES];
    
    if (!event)
      break;
    else
      [NSApp sendEvent: event];
  }
}

} // namespace Neptune
