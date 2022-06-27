#pragma once

#include "renderer/RenderAPI.h"
#include "renderer/RenderContext.h"

namespace Neptune
{

// ----- MacRenderContext ---------

class MacRenderContext : public RenderContext
{
public:
  static Ref<MacRenderContext> Create(RenderAPI api, Size width, Size height);
  virtual ~MacRenderContext();
};

// ----- MacMetalContext ---------

class MacMetalRenderContext final : public MacRenderContext
{
public:
  MacMetalRenderContext(Size width, Size height);
  virtual ~MacMetalRenderContext();
  
  void* GetLayer() noexcept { return m_Layer; }
  
private:
  void* m_Layer;
};

} // namesapce Neptune
