#pragma once

#ifdef NEPTUNE_MACOS

#include "Application.h"

namespace Neptune
{
  extern Application* CreateApplication();
}

int main()
{
  auto app = Neptune::CreateApplication();
  app->Run();
  delete app;
  
  return 0;
}

#endif // NEPTUNE_MACOS
