#include "neptunepch.h"
#include "MacRenderContext.h"

#include "metal/MetalRenderDevice.h"
#include "metal/MetalSwapchain.h"

#include "renderer/RenderCommand.h"

#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

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
  Ref<MetalRenderDevice> device = CreateRef<MetalRenderDevice>();
  
  m_Device = device;
  RenderCommand::OnInit(m_Device);
  
  m_Layer = [CAMetalLayer layer];
  ((CAMetalLayer*)m_Layer).device = (id<MTLDevice>)device->GetDevice();
  
  m_Swapchain = CreateRef<MetalSwapchain>((id<MTLDevice>)device->GetDevice(), (CAMetalLayer*)m_Layer);
}

MacMetalRenderContext::~MacMetalRenderContext()
{
  RenderCommand::OnTerminate();
}

} // namespace Neptune
