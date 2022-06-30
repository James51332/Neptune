#include "neptunepch.h"
#include "SandboxLayer.h"

#include <imgui/imgui.h>

SandboxLayer::SandboxLayer()
	: Layer("Sandbox Layer")
{
  
}

SandboxLayer::~SandboxLayer()
{
  
}

void SandboxLayer::OnInit(const Neptune::Ref<Neptune::RenderDevice>& device)
{
  m_RenderDevice = device;
  
  // Create framebuffers
  {
    Neptune::FramebufferDesc desc;
    desc.Width = m_ViewportSize.x;
    desc.Height = m_ViewportSize.y;
    
    for (Neptune::Size i = 0; i < Neptune::Renderer::GetMaxFramesInFlight(); i++)
      m_Framebuffers.PushBack(m_RenderDevice->CreateFramebuffer(desc));
  }
  
  // Load Texture
  m_Texture = m_RenderDevice->LoadTexture("resources/panda.png");
  
  // Create Camera
  {
    Neptune::CameraDesc desc;
    desc.Type = Neptune::ProjectionType::Orthographic;
    desc.Position = { 0.0f, 0.0f, 0.0f };
    desc.Zoom = 0.5f;
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    desc.Near = -10.0f;
    desc.Far = 10.0f;
    m_Camera = Neptune::Camera(desc);
  }
}

void SandboxLayer::OnTerminate()
{
  
}

void SandboxLayer::OnUpdate()
{
  // TODO: Camera Controller
  // Update Camera
  {
    Neptune::Float3 translate = Neptune::Float3(0.0f);
    
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyD)) translate.x += 0.05f;
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyA)) translate.x -= 0.05f;
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyW)) translate.y += 0.05f;
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyS)) translate.y -= 0.05f;
    
    Neptune::CameraDesc desc = m_Camera.GetDesc();
    desc.Position += translate;
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    m_Camera.SetDesc(desc);
  }
  
  // Resize Framebuffer
  {
    Neptune::FramebufferDesc desc = m_Framebuffers[Neptune::Renderer::GetFrameNumber()]->GetDesc();
    if (desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y)
      m_Framebuffers[Neptune::Renderer::GetFrameNumber()]->Resize(m_ViewportSize.x, m_ViewportSize.y);
  }
}

void SandboxLayer::OnRender(const Neptune::Ref<Neptune::Framebuffer>& framebuffer)
{
  // Scene Pass
  {
    Neptune::RenderPass scenePass;
    {
      scenePass.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
      scenePass.LoadAction = Neptune::LoadAction::Clear;
      scenePass.StoreAction = Neptune::StoreAction::Store;
      scenePass.Framebuffer = m_Framebuffers[Neptune::Renderer::GetFrameNumber()];
    }
    Neptune::RenderCommand::BeginRenderPass(scenePass);
    
    Neptune::Renderer2D::Begin(m_Camera);
    Neptune::Renderer2D::DrawQuad(Neptune::Matrix4(1.0f), m_Texture);
    Neptune::Renderer2D::End();
    
    Neptune::RenderCommand::EndRenderPass();
  }
  
  // ImGui (Swapchain Pass)
  {
    Neptune::RenderPass renderPass;
    {
      renderPass.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
      renderPass.LoadAction = Neptune::LoadAction::Clear;
      renderPass.StoreAction = Neptune::StoreAction::Store;
      renderPass.Framebuffer = framebuffer;
    }
    Neptune::RenderCommand::BeginRenderPass(renderPass);
    
    Neptune::ImGUIRenderer::Render();
    
    Neptune::RenderCommand::EndRenderPass();
  }
}

void SandboxLayer::OnImGuiRender()
{
  // ImGui
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
    ImGui::Begin("Viewport");
    
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)&m_Framebuffers[Neptune::Renderer::GetFrameNumber()]->GetColorAttachment(), ImVec2(m_ViewportSize.x, m_ViewportSize.y));
    m_ViewportSize = { viewSize.x, viewSize.y };
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
}

void SandboxLayer::OnEvent(Neptune::Scope<Neptune::Event>& e)
{
  
}
                     
