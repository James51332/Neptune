#pragma once

namespace Neptune
{

// ----- CommandBuffer -----------------

// CommandBuffers are sequences of commands that are sent to the GPU.
// They should be stack allocated and should only be the size of their
// ID. They aren't reusable and can be recorded into via RenderCommand.

struct CommandBuffer
{
  Size ID;
};

} // namespace Neptune
