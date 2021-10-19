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

class MacMetalRenderContext : public MacRenderContext
{
  friend class MacWindow;
public:
  MacMetalRenderContext();
  virtual ~MacMetalRenderContext();
  
private:
  void* m_Layer;
};

} // namesapce Neptune
