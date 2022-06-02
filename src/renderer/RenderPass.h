#pragma once

#include "Swapchain.h"

namespace Neptune
{

// ----- LoadAction -----------------

enum class LoadAction
{
  Clear = 0,
  Load,
  DontCare
};

// ----- RenderPass -----------------

struct RenderPass
{
  LoadAction LoadAction;
  Ref<Framebuffer> Framebuffer;
  Float4 ClearColor;
};

} // namespace Neptune

