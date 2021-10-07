#include "neptunepch.h"
#include <Neptune/Neptune.h>

class SandboxApp : public Neptune::Application
{
public:
  SandboxApp()
  	: Application({1280, 720, "Sandbox"})
  {
  }
  
  ~SandboxApp()
  {
  }
};

Neptune::Application* Neptune::CreateApplication()
{
  return new SandboxApp();
}
