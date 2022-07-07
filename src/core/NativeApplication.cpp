#include "neptunepch.h"
#include "NativeApplication.h"

//#include "macos/MacApplication.h"
#include "windows/WindowsApplication.h"

namespace Neptune
{

Scope<NativeApplication> NativeApplication::Create()
{
#ifdef NEPTUNE_MACOS
  return CreateScope<MacApplication>();
#endif

#ifdef NEPTUNE_WINDOWS
  return CreateScope<WindowsApplication>();
#endif
}

NativeApplication::~NativeApplication()
{
  
}

} // namespace Neptune
