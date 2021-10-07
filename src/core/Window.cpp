#include "neptunepch.h"
#include "Window.h"

#include "macos/MacWindow.h"

namespace Neptune
{

Scope<Window> Window::Create(const WindowDesc& desc)
{
#ifdef NEPTUNE_MACOS
  return CreateScope<MacWindow>(desc);
#endif
}

Window::~Window()
{
  
}

} // namespace Neptune
