#pragma once

#include "renderer/RenderAPI.h"
#include "renderer/RenderContext.h"

namespace Neptune
{

// ----- MacRenderContext ---------

class MacRenderContext : public RenderContext
{
public:
  static Ref<MacRenderContext> Create(RenderAPI api);
  virtual ~MacRenderContext();
};

// ----- MacMetalContext ---------

class MacMetalRenderContext final : public MacRenderContext
{
public:
  MacMetalRenderContext();
  virtual ~MacMetalRenderContext();
  
  void* GetLayer() noexcept { return m_Layer; }
  
private:
  void* m_Layer;
};

} // namesapce Neptune
