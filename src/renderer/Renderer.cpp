#include "neptunepch.h"
#include "Renderer.h"

namespace Neptune
{

RenderAPI Renderer::s_API = RenderAPI::Metal; // TODO: Expose this to Application

void Renderer::OnInit()
{
  
}

void Renderer::OnShutdown()
{
  
}

RenderAPI Renderer::GetAPI() noexcept
{
  return s_API;
}

} // namespace Neptune
