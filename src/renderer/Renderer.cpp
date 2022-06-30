#include "neptunepch.h"
#include "Renderer.h"

#include "renderer/Renderer2D.h"
#include "renderer/ImGUIRenderer.h"

namespace Neptune
{

RenderAPI Renderer::s_API = RenderAPI::Metal; // TODO: Expose this to Application
Size Renderer::s_Frame = 0;

void Renderer::OnInit(const Ref<RenderDevice>& device, Size width, Size height)
{
  ImGUIRenderer::OnInit(device, width, height);
  Renderer2D::OnInit(device);
}

void Renderer::OnUpdate()
{
  s_Frame = (s_Frame + 1) % s_FramesInFlight;
}

void Renderer::OnTerminate()
{
  Renderer2D::OnTerminate();
  ImGUIRenderer::OnTerminate();
}

void Renderer::OnEvent(Scope<Event> &event)
{
  ImGUIRenderer::OnEvent(event);
}

RenderAPI Renderer::GetAPI() noexcept
{
  return s_API;
}

Size Renderer::GetMaxFramesInFlight() noexcept
{
  return s_FramesInFlight;
}

Size Renderer::GetFrameNumber() noexcept
{
  return s_Frame;
}

} // namespace Neptune
