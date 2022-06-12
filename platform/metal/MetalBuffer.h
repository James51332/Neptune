#pragma once

#include "renderer/Buffer.h"

#import <Metal/MTLBuffer.h>

namespace Neptune
{

// ----- MetalBuffer -----------------

class MetalBuffer final : public Buffer
{
public:
  MetalBuffer(id<MTLDevice> device, const BufferDesc& desc);
  ~MetalBuffer();
  
  id<MTLBuffer> GetBuffer() { return m_Buffer; }
  
  void Update(Size size, const void* data);
  
private:
  id<MTLBuffer> m_Buffer;
};

} // namespace Neptune
