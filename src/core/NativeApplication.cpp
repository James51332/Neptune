#include "neptunepch.h"
#include "NativeApplication.h"

#include "macos/MacApplication.h"

namespace Neptune
{

Scope<NativeApplication> NativeApplication::Create()
{
#ifdef NEPTUNE_MACOS
  return CreateScope<MacApplication>();
#endif
}

NativeApplication::~NativeApplication()
{
  
}

} // namespace Neptune
