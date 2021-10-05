#pragma once

#include "core/EventQueue.h"

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
};

} // namespace Neptune
