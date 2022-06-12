#pragma once

#include "utils/SmartPointer.h"
#include "utils/Utils.h"

#include "core/EventQueue.h"
#include "core/NativeApplication.h"
#include "core/Window.h"

#include "renderer/RenderContext.h"

namespace Neptune
{

class Application
{
public:
  Application(const WindowDesc& desc = WindowDesc(1280, 720, "Neptune"));
  virtual ~Application();
  
  void Run();
  void Stop() noexcept { m_Running = false; }
  
  static Application* GetSingleton() noexcept;
  static void PushEvent(Scope<Event> e) noexcept
  {
    GetSingleton()->GetEventQueue().PushEvent(Move(e));
  }
  
  EventQueue& GetEventQueue() noexcept { return m_EventQueue; }
  
private:
  static Application* s_Application;

private:
  bool m_Running = false;
  
  EventQueue m_EventQueue;
  Scope<NativeApplication> m_NativeApp;
  Scope<Window> m_Window;
  Ref<RenderContext> m_RenderContext;
  Ref<RenderDevice> m_RenderDevice;
  Ref<Swapchain> m_Swapchain;
};

} // namespace Neptune
