#pragma once
#include "renderer/Sync.h"

#import <Metal/MTLCommandBuffer.h>
#import <Metal/MTLEvent.h>

namespace Neptune
{

// ----- MetalSemaphore -----------------

// Allows for GPU execution dependency. This is much
// more useful between queues, however, it won't break
// if used withing a single queue. The problem we run into
// is that a signal scheduled after a wait will reach a deadlock.
// If we schedule the signal before, the wait, since everything
// is chronological, it's redundant. We won't rely on any synchronization
// primitives until we use multiple queues.
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
  
  void Signal();
  void Reset();
  void Wait(Float32 timeoutInMs = 0.0f);
  
  bool Status();
  
private:
  bool m_Signaled;
  Mutex m_SignaledMutex;
  ConditionVariable m_ConditionVariable;
};

} // namespace Neptune
