#include "neptunepch.h"
#include "WindowsApplication.h"

#include <Windows.h>

namespace Neptune
{

// ----- WindowsApplication -----------------

WindowsApplication::WindowsApplication()
{
}

WindowsApplication::~WindowsApplication()
{
}

void WindowsApplication::PollEvents()
{
  MSG msg;
  while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

} // namespace Neptune