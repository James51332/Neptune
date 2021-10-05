#include "neptunepch.h"
#include "Application.h"

#define NEPTUNE_ENTRYPOINT
#include "EntryPoint.h"

namespace Neptune
{

Application* Application::s_Application = nullptr;

Application::Application()
{
  NEPTUNE_ASSERT(!s_Application, "Only one instance of application can be created!");
  s_Application = this;
}

Application::~Application()
{
  
}

void Application::Run()
{
  m_Running = true;
  while (m_Running)
  {
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      // Dispatch Events
    }
  }
}

Application* Application::GetSingleton() noexcept
{
  return s_Application;
}

} // namespace Neptune
