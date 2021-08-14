#include <Neptune/Neptune.h>

class SandboxApp : public Neptune::Application
{
public:
  SandboxApp()
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
