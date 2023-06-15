#pragma once

#include "utils/SmartPointer.h"
#include "utils/Utils.h"

#include "core/EventQueue.h"
#include "core/NativeApplication.h"
#include "core/Window.h"
#include "core/LayerStack.h"
#include "core/Timestep.h"

#include "renderer/RenderContext.h"
#include "renderer/Swapchain.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"

namespace Neptune
{

class Application
{
public:
  Application(const WindowDesc& desc = WindowDesc(1280, 720, "Neptune"));
  virtual ~Application();
  
  void Run();
  void Stop() noexcept { m_Running = false; }
  void PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }
  
  static Application* GetSingleton() noexcept;
  static Float32 PollTime() { return GetSingleton()->m_NativeApp->PollTime(); }
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
  
  LayerStack m_LayerStack;
  
  Ref<RenderContext> m_RenderContext;
  Ref<RenderDevice> m_RenderDevice;
  Ref<Swapchain> m_Swapchain;
  
  Timestep m_LastTime;
};

} // namespace Neptune
