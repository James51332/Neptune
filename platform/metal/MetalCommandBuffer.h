#pragma once

#include "renderer/CommandBuffer.h"
#include "renderer/Sync.h"

#import <Metal/MTLCommandQueue.h>
#import <Metal/MTLCommandBuffer.h>

namespace Neptune
{

class MetalFence;

// ----- MetalCommandBufferRegistry -----------------

// CommandBuffers are just IDs. This allows very easy high-level abstraction.
// In Metal, command buffers are transient. In vulkan, we can reuse them.
// We're gonna operate this like a heap. We'll keep track of free slots.
// When all our slots are used, we'll just expand the registry by one. Realistically,
// our registry should stay under the smallest size for the array (8) and we'll
// never need to copy or reallocate anything.

class MetalCommandBufferRegistry
{
public:
  MetalCommandBufferRegistry(id<MTLCommandQueue> queue);
  ~MetalCommandBufferRegistry();
  
  void Register(CommandBuffer& buffer);
  id<MTLCommandBuffer> Get(CommandBuffer buffer);
  void Commit(CommandBuffer buffer, Ref<MetalFence>& idleFence);
  void Commit(CommandBuffer buffer, const Ref<Fence>& fence, Ref<MetalFence>& idleFence);
  void Free(CommandBuffer buffer);
  
private:
  id<MTLCommandQueue> m_Queue;
  
  DynamicArray<id<MTLCommandBuffer>> m_Registry;
  DynamicArray<Size> m_Open;
};

} // namespace Neptune
