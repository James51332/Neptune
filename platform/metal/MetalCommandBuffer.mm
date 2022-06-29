#include "neptunepch.h"
#include "MetalCommandBuffer.h"

#include "MetalSync.h"

namespace Neptune
{

// ----- MetalCommandBufferRegistry -----------------

MetalCommandBufferRegistry::MetalCommandBufferRegistry(id<MTLCommandQueue> queue)
: m_Queue(queue)
{
}

MetalCommandBufferRegistry::~MetalCommandBufferRegistry()
{
  @autoreleasepool
  {
  	for (auto& buffer : m_Registry)
  		buffer = nil;
  }
}

void MetalCommandBufferRegistry::Register(CommandBuffer& buffer)
{
  @autoreleasepool
  {
  	id<MTLCommandBuffer> buf = [[m_Queue commandBuffer] retain];
  	Size ID;
  	
  	// If there is free memory in the array, use it. Otherwise, push it to the back.
  	if (!m_Open.Empty())
  	{
  	  ID = m_Open[m_Open.Size() - 1];
  	  m_Registry[ID] = buf;
  	  m_Open.PopBack();
  	} else
  	{
  	  ID = m_Registry.Size();
  	  m_Registry.PushBack(buf);
  	}
  	
  	buffer.ID = ID;
  }
}

id<MTLCommandBuffer> MetalCommandBufferRegistry::Get(CommandBuffer buffer)
{
  // TODO: Not having debug asserts on arrays is an actual security issue so I'll fix it soon.
  return m_Registry[buffer.ID];
}

void MetalCommandBufferRegistry::Commit(CommandBuffer buffer, Ref<MetalFence>& idleFence)
{
  NEPTUNE_ASSERT(m_Registry[buffer.ID], "Invalid command buffer");
  
  @autoreleasepool
  {
    [m_Registry[buffer.ID] addCompletedHandler: ^(id<MTLCommandBuffer> buffer) {
      idleFence->Signal();
    }];
    
  	[m_Registry[buffer.ID] commit];
  	Free(buffer); // We can free after committing.
  }
}

void MetalCommandBufferRegistry::Commit(CommandBuffer buffer, const Ref<Fence>& fence, Ref<MetalFence>& idleFence)
{
  NEPTUNE_ASSERT(m_Registry[buffer.ID], "Invalid command buffer");
  
  @autoreleasepool
  {
    [m_Registry[buffer.ID] addCompletedHandler: ^(id<MTLCommandBuffer> buffer) {
      StaticRefCast<MetalFence>(fence)->Signal();
      idleFence->Signal();
    }];
    [m_Registry[buffer.ID] commit];
    Free(buffer); // We can free after committing.
  }
}

void MetalCommandBufferRegistry::Free(CommandBuffer buffer)
{
  @autoreleasepool
  {
  	// Release old buffer
    [m_Registry[buffer.ID] release];
  	m_Registry[buffer.ID] = nil;
  	
  	// There is free space in array that can now be written to.
  	if (buffer.ID != m_Registry.Size() - 1)
  	  m_Open.PushBack(buffer.ID);
  	else
  	  m_Registry.PopBack();
  }
}

} // namespace Neptune
