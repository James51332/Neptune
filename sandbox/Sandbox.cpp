#include "neptunepch.h"

#include "SandboxLayer.h"

#include <Neptune/Neptune.h>

class SandboxApp : public Neptune::Application
{
public:
  SandboxApp()
  	: Application({1280, 720, "Sandbox", true})
  {
  }
  
  ~SandboxApp()
  {
  }
};

Neptune::Application* Neptune::CreateApplication()
{
  Neptune::Application* app = new SandboxApp();
  app->PushLayer(new SandboxLayer());
  return app;
}
