#include "neptunepch.h"
#include "MetalBuffer.h"

#import <Metal/MTLDevice.h>

namespace Neptune
{

// ----- MetalBuffer -----------------

MetalBuffer::MetalBuffer(id<MTLDevice> device, const BufferDesc& desc)
	: m_Buffer(nil), Buffer(desc.Size, desc.Usage, desc.Type)
{
  if (desc.Data)
  {
    m_Buffer = [device newBufferWithBytes: desc.Data
                                   length: desc.Size
                                  options: MTLStorageModeShared];
  } else
  {
    m_Buffer = [device newBufferWithLength: desc.Size
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
 
  // I'm not totally sure if this is a safe and secure operation.
  memcpy(m_Buffer.contents, data, size);
}

} // namspace Neptune
