#pragma once

namespace Neptune
{

// ----- Semaphore -----------------

// A semaphore is a synchronization primitive which ensures execution order (GPU).
// It can be either signaled or unsignaled. We are implementing this according
// to the Vulkan Specification. We must ensure that a semaphore is signaled or
// has a pending signal before we wait for it (7.4.3).

// This struct is reserved, but Vulkan doesn't actually need any important information.
struct SemaphoreDesc
{
};

class Semaphore
{
public:
  virtual ~Semaphore() = default;
};

// ----- Fence -----------------

// A fence is a synchronization primitive for CPU-GPU dependency. It can be signaled
// or unsignaled, and can be submitted with command buffers to allow the CPU to wait
// for a signal. By default, fences are created in the unsignaled state, but we should
// be able to change that. We should create an API to wait and unsignal it from the CPU
// via a RenderDevice.

struct FenceDesc
{
  bool Signaled = false;
};

class Fence
{
public:
  virtual ~Fence() = default;
};

} // namespace Neptune
