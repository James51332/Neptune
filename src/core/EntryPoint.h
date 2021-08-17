#pragma once

#include "core/Application.h"

namespace Neptune
{
extern Application* CreateApplication();
}

#ifdef NEPTUNE_ENTRYPOINT

#ifdef NEPTUNE_MACOS

int main()
{
  auto app = Neptune::CreateApplication();
  app->Run();
  delete app;
  
  return 0;
}

#endif // NEPTUNE_MACOS

#endif // NEPTUNE_ENTRYPOINT