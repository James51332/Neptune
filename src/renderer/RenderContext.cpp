#include "neptunepch.h"
#include "RenderContext.h"

#include "renderer/Renderer.h"

#include "macos/MacRenderContext.h"

namespace Neptune
{

Ref<RenderContext> RenderContext::Create()
{
#ifdef NEPTUNE_MACOS
  return MacRenderContext::Create(Renderer::GetAPI());
#endif
}

RenderContext::~RenderContext() noexcept
{
  
}

} // namespace Neptune
