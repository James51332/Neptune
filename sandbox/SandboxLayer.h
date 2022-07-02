#pragma once

#include <Neptune/Neptune.h>

// ----- SandboxLayer -----------------

class SandboxLayer : public Neptune::Layer
{
public:
  SandboxLayer();
  ~SandboxLayer();
  
  virtual void OnInit(const Neptune::Ref<Neptune::RenderDevice>& device) override;
  virtual void OnTerminate() override;
  virtual void OnUpdate() override;
  virtual void OnRender(const Neptune::Ref<Neptune::Framebuffer>& framebuffer) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Neptune::Scope<Neptune::Event>& e) override;
  
private:
  Neptune::Ref<Neptune::RenderDevice> m_RenderDevice;
  
  Neptune::DynamicArray<Neptune::Ref<Neptune::Framebuffer>> m_Framebuffers;
  Neptune::Float2 m_ViewportSize = { 800, 600 };
  
  Neptune::Ref<Neptune::Texture> m_Texture;
  Neptune::Camera m_Camera;

  Neptune::Ref<Neptune::Buffer> m_VB;
  Neptune::Ref<Neptune::Buffer> m_IB;
  Neptune::Ref<Neptune::Buffer> m_UB;
  Neptune::Ref<Neptune::PipelineState> m_Pipeline;
};
