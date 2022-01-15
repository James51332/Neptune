#include "neptunepch.h"
#include "MetalRenderDevice.h"

#include <Metal/MTLDevice.h>
#include <Metal/MTLCommandQueue.h>

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

} // namespace Neptune
