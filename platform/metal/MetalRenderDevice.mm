#include "neptunepch.h"
#include "MetalRenderDevice.h"

#include "MetalShader.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLCommandQueue.h>

namespace Neptune
{

MetalRenderDevice::MetalRenderDevice()
{
  m_Device = MTLCreateSystemDefaultDevice(); // retained
  m_Queue = [((id<MTLDevice>)m_Device) newCommandQueue];
}

MetalRenderDevice::~MetalRenderDevice()
{
  [(id<MTLDevice>)m_Device release]; // retained
  [(id<MTLCommandQueue>)m_Queue release];
}

Ref<Shader> MetalRenderDevice::CreateShader(const ShaderDesc& desc)
{
  Ref<Shader> shader = CreateRef<MetalShader>((id<MTLDevice>)m_Device, desc);
  
  // This prevents shaders from deleting before were finished.
  // If a shader needs to explicitly be destroyed, we'll have to adjust.
  m_Shaders.PushBack(shader);
  
  return shader;
}

} // namespace Neptune
