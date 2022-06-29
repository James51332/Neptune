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
  Size GetOffset() { return (m_Offset * m_Size); }
  
  void Update(Size size, const void* data);
  
private:
  id<MTLBuffer> m_Buffer;
  Size m_Offset = 0;
};

} // namespace Neptune
