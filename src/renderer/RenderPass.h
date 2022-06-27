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

// ----- StoreAction -----------------

enum class StoreAction
{
  Store,
  DontCare
};

// ----- RenderPass -----------------

struct RenderPass
{
  LoadAction LoadAction;
  StoreAction StoreAction;
  Ref<Framebuffer> Framebuffer;
  Float4 ClearColor;
};

} // namespace Neptune

