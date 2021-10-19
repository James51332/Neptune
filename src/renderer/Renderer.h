#pragma once

#include "renderer/RenderContext.h"

namespace Neptune
{

class Renderer
{
public:
  static void OnInit();
  static void OnShutdown();
  
  static RenderAPI GetAPI() noexcept;
  
private:
  static RenderAPI s_API;
};

} // namespace Neptune
