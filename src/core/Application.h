#pragma once

namespace Neptune
{

class Application
{
public:
  Application();
  virtual ~Application();
  
  void Run();
  
  static Application* GetSingleton() noexcept;
  
private:
  static Application* s_Application;
};

} // namespace Neptune
