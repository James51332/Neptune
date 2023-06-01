#pragma once

#include "Panel.h"

#include "renderer/RenderDevice.h"

namespace Neptune
{

class Viewport : public Panel
{
public:
  Viewport(Scene* scene, Ref<RenderDevice>& device);
  ~Viewport();
  
  virtual void OnImGuiRender() override;
  virtual void OnUpdate() override;
  void OnRender();
  
private:
  DynamicArray<Ref<Framebuffer>> m_Framebuffers;
  Float2 m_ViewportSize {800, 600};
  
  CameraController m_CameraController;
};


} // namespace Neptune
