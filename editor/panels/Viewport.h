#pragma once

#include "Panel.h"

#include "renderer/RenderDevice.h"

namespace Neptune
{

class Viewport : public Panel
{
public:
  Viewport(Ref<Scene> scene, Ref<RenderDevice>& device);
  ~Viewport();
  
  virtual void OnImGuiRender() override;
  virtual void OnUpdate(Timestep ts) override;
  void OnRender();
  
private:
  DynamicArray<Ref<Framebuffer>> m_Framebuffers;
  Float2 m_ViewportSize {800, 600};
  bool m_Focused;
  
  CameraController m_CameraController;
};


} // namespace Neptune
