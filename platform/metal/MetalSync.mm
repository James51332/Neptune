#include "neptunepch.h"
#include "MetalSync.h"

namespace Neptune
{

// ----- MetalSemaphore -----------------

MetalSemaphore::MetalSemaphore(id<MTLDevice> device, const SemaphoreDesc& desc)
	: m_Value(1), m_Event([device newEvent]) // retained
{
  
}

MetalSemaphore::~MetalSemaphore()
{
  @autoreleasepool
  {
		[m_Event release]; // retained
  }
}

// ----- Design -----------------

// The value stored in m_Value should start at one because we don't want
// to start in the signaled state. After a signal, the next wait doesn't
// wait on the GPU at all because it waits for the value already stored
// in the event. This is the desired behavior, and it's attained by using
// post-increment when determining the value to wait for. Here are two
// examples, of the order that signals and waits may be executed. We'll
// require that signals are encoded before waits. All of this theory is nice
// but from an implementation standpoint, we really don't need more than one
// any time soon. We'll require that a user uses fences to synchronize because
// deferred signals will just stall a single queue.

// ----- Valid -----------------

// Event | Value
// ------|------
// 0     | 1     Start
// 0     | 1     EncodeSignal (1)
// 1     | 1     Signal
// 1     | 2     EncodeWait (1) Skip

// Event | Value
// ------|------
// 0     | 1     Start
// 0     | 1     EncodeSignal
// 0     | 2     EncodeWait (1) Wait
// 1     | 2     Signal (End Wait)

// Event | Value
// ------|------
// 0     | 1     Start
// 0     | 1     DeferredSignal
// 0     | 2     EncodeWait (1) Wait
// 0     | 2     EncodeSignal (Deferred-1) Wait
// 1     | 2     Signal (Seperate Queue) (End Wait)

// ----- Invalid -----------------

// Event | Value
// ------|------
// 0     | 1     Start
// 0     | 2     EncodeWait (1) Wait
// 0     | 2     EncodeSignal
// 2     | 2     Signal

// ----- Deferred Signals -----------------

// Sometimes, we won't have access to a command buffer right away even though
// a signal semaphore is given to the API. For example, when we acquire a
// swapchain image. A swapchain image will become available once it's presented.
// Since we need to have an API wait on semaphores when we present a swapchain image,
// we'll need to use a commnad buffer to present it. This means that our semaphore won't
// be encoded to signal until the image becomes avaiable, which may be after the
// wait is encoded. We can circumvent this by only signaling to the value stored
// before the wait, which can be cached easily. This must be done into a separate
// queue though becuase the queue that is waiting will be blocked. This may be useful,
// but supporting multiple queues isn't a huge priority for us because utilitizing
// them maximally will be challenging. For now, we'll just enforce that the user can't
// submit a command buffer that waits on the acquisition of an image. This shouldn't
// affect performance at all because we can still implement triple buffering.

void MetalSemaphore::EncodeSignal(id<MTLCommandBuffer> buffer)
{
	[buffer encodeSignalEvent: m_Event
                      value: m_Value];
}

void MetalSemaphore::EncodeWait(id<MTLCommandBuffer> buffer)
{
	[buffer encodeWaitForEvent: m_Event
                       value: m_Value++];
}

void MetalSemaphore::EncodeDeferred(id<MTLCommandBuffer> buffer, Size value)
{
  [buffer encodeSignalEvent: m_Event
                      value: value];
}

// ----- MetalFence -----------------

MetalFence::MetalFence(id<MTLDevice> device, const FenceDesc& desc)
	: m_Signaled(desc.Signaled)
{
}

MetalFence::~MetalFence()
{
}

void MetalFence::Signal() noexcept
{
  m_Signaled = true;
}

void MetalFence::Reset() noexcept
{
  m_Signaled = false;
}

} // namespace Neptune
