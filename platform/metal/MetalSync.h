#pragma once
#include "renderer/Sync.h"

#include <Metal/MTLCommandBuffer.h>
#include <Metal/MTLEvent.h>

namespace Neptune
{

// ----- MetalSemaphore -----------------

// Allows for GPU execution dependency. This is much
// more useful between queues, however, it won't break
// if used withing a single queue.
class MetalSemaphore final : public Semaphore
{
public:
  MetalSemaphore(id<MTLDevice> device, const SemaphoreDesc& desc);
  ~MetalSemaphore();

  void EncodeSignal(id<MTLCommandBuffer> buffer);
  void EncodeWait(id<MTLCommandBuffer> buffer);
  
  Size DeferredValue() const noexcept { return m_Value; }
  void EncodeDeferred(id<MTLCommandBuffer> buffer, Size value);
  
private:
  id<MTLEvent> m_Event;
  Atomic<Size> m_Value;
};

// ----- MetalFence -----------------

// A fence in metal is implemented via  an atomic boolean.
// MTLFences sounds appealling but are very different from
// VkFences, and therefore, our fence primitive.
class MetalFence final : public Fence
{
public:
  MetalFence(id<MTLDevice> device, const FenceDesc& desc);
  ~MetalFence();
  
  void Signal() noexcept;
  void Reset() noexcept;
  
private:
  Atomic<bool> m_Signaled;
};

} // namespace Neptune
