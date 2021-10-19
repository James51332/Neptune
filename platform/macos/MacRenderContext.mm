#include "neptunepch.h"
#include "MacRenderContext.h"

#include <Cocoa/Cocoa.h>
#include <Metal/Metal.h>
#include <QuartzCore/CAMetalLayer.h>

namespace Neptune
{

// ----- MacRenderContext ---------

Ref<MacRenderContext> MacRenderContext::Create(RenderAPI api)
{
  switch (api) {
    case RenderAPI::None: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); break; }
    case RenderAPI::OpenGL: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); break; }
    case RenderAPI::Metal: { return CreateRef<MacMetalRenderContext>(); break; }
    case RenderAPI::Vulkan: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); break; }
    case RenderAPI::DirectX: { NEPTUNE_ASSERT(false, "Unsupported RenderAPI on this platform!"); break; }
  }
  return nullptr;
}

MacRenderContext::~MacRenderContext()
{
  
}

// ----- MacMetalRenderContext ---------

MacMetalRenderContext::MacMetalRenderContext()
{
  m_API = RenderAPI::Metal;
  m_Layer = [CAMetalLayer layer];
  ((CAMetalLayer*)m_Layer).device = MTLCreateSystemDefaultDevice();
}

MacMetalRenderContext::~MacMetalRenderContext()
{
  
}

} // namespace Neptune
