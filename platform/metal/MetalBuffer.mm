#include "neptunepch.h"
#include "MetalBuffer.h"

#import <Metal/MTLDevice.h>

namespace Neptune
{

// ----- MetalBuffer -----------------

MetalBuffer::MetalBuffer(id<MTLDevice> device, const BufferDesc& desc)
	: m_Buffer(nil), Buffer(desc.Size, desc.Usage, desc.Type)
{
  // TODO: Extract triple buffering constant to global
  if (desc.Data)
  {
    m_Buffer = [device newBufferWithBytes: desc.Data
                                   length: desc.Size * (desc.Usage == BufferUsage::Dynamic ? 3 : 1)
                                  options: MTLStorageModeShared];
  } else
  {
    m_Buffer = [device newBufferWithLength: desc.Size * (desc.Usage == BufferUsage::Dynamic ? 3 : 1)
                                   options: MTLStorageModeShared];
  }
  
  NEPTUNE_ASSERT(m_Buffer, "Failed to create buffer!");
}

MetalBuffer::~MetalBuffer()
{
  [m_Buffer release];
  m_Buffer = nil;
}

void MetalBuffer::Update(Size size, const void* data)
{
  NEPTUNE_ASSERT(m_Usage == BufferUsage::Dynamic, "Only dynamic buffers can be updated!");
  NEPTUNE_ASSERT(size <= m_Size, "Data size larger than buffer size!");

  // TODO: This should be globalized
  static constexpr Size framesInFlight = 3;
  m_Offset = (m_Offset + 1) % framesInFlight;
  
  // This is definitely not well-defined anymore.
  // Reinterpret to cast to offset by specified bytes.
  void* dest = (void*)(reinterpret_cast<char*>(m_Buffer.contents) + (m_Size * m_Offset));
  memcpy(dest, data, size);
}

} // namspace Neptune
