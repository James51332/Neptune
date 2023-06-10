#pragma once

#include "core/Event.h"
#include "core/Timestep.h"

#include "renderer/RenderDevice.h"
#include "renderer/Swapchain.h"

namespace Neptune
{

// ----- Layer -----------------

class Layer
{
public:
  virtual ~Layer() = default;
  
  virtual void OnInit(const Neptune::Ref<Neptune::RenderDevice>& device) {}
  virtual void OnTerminate() {}
  virtual void OnUpdate(Timestep ts) {}
  virtual void OnRender(const Ref<Framebuffer>& framebuffer) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Scope<Event>& e) {}
  
  const String& GetDebugName() const noexcept { return m_DebugName; }
  
protected:
  Layer(const String& name = "Layer")
  	: m_DebugName(name) {}
  
private:
  String m_DebugName;
};

} // namespace Neptune
