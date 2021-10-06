#pragma once

#include "utils/SmartPointer.h"

#include "core/EventQueue.h"
#include "core/NativeApplication.h"

namespace Neptune
{

class Application
{
public:
  Application();
  virtual ~Application();
  
  void Run();
  void Stop() noexcept { m_Running = false; }
  
  static Application* GetSingleton() noexcept;
  EventQueue& GetEventQueue() noexcept { return m_EventQueue; }
  
private:
  static Application* s_Application;

private:
  bool m_Running = false;
  
  EventQueue m_EventQueue;
  Scope<NativeApplication> m_NativeApp;
};

} // namespace Neptune
