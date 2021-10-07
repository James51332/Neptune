#include "neptunepch.h"
#include "Application.h"

#define NEPTUNE_ENTRYPOINT
#include "EntryPoint.h"

#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"

namespace Neptune
{

Application* Application::s_Application = nullptr;

Application::Application()
{
  NEPTUNE_ASSERT(!s_Application, "Only one instance of application can be created!");
  s_Application = this;
  
  m_NativeApp = NativeApplication::Create();
  m_Window = Window::Create();
}

Application::~Application()
{
  
}

void Application::Run()
{
  
  // Initialization
  m_Window->Show();
  
  m_Running = true;
  while (m_Running)
  {
    m_NativeApp->PollEvents();
    
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      // Dispatch Events
      m_EventQueue.Dispatch<WindowClosedEvent>(e, [this](const WindowClosedEvent& event) {
        Stop();
        return true;
      });
    }
  }
  
  // Shutdown
  m_Window->Close();
}

Application* Application::GetSingleton() noexcept
{
  return s_Application;
}

} // namespace Neptune
