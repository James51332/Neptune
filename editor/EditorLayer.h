#pragma once

#include <Neptune/Neptune.h>

// ----- EditorLayer -----------------

namespace Neptune
{

class EditorLayer : public Layer
{
public:
  EditorLayer();
  ~EditorLayer();
  
  virtual void OnInit(const Ref<RenderDevice>& device) override;
  virtual void OnTerminate() override;
  virtual void OnUpdate() override;
  virtual void OnRender(const Ref<Framebuffer>& framebuffer) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Scope<Event>& e) override;
  
private:
  Ref<RenderDevice> m_RenderDevice;
  
  DynamicArray<Ref<Framebuffer>> m_Framebuffers;
  Float2 m_ViewportSize = { 800, 600 };
  
  Ref<Texture> m_Texture; 
  CameraController m_CameraController;
  
  Ref<Mesh> m_Mesh;
  Ref<Material> m_Material;
};

} // namespace Neptune
