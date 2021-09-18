#pragma once

#include "core/Application.h"
#include "core/PlatformDetector.h"

namespace Neptune
{
extern Application* CreateApplication();
}

#ifdef NEPTUNE_ENTRYPOINT

#ifdef NEPTUNE_MACOS

int main()
{
  Neptune::Logger::Initialize();
 	NEPTUNE_CORE_INFO("Engine Initializing...");
  
  auto app = Neptune::CreateApplication();
  app->Run();
  delete app;
  
  return 0;
}

#endif // NEPTUNE_MACOS

#endif // NEPTUNE_ENTRYPOINT
