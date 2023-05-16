#include "neptunepch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define GLM_DEPTH_FORCE_ONE_TO_ZERO
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{

EditorLayer::EditorLayer()
: Layer("Editor Layer")
{
}

EditorLayer::~EditorLayer()
{
}

void EditorLayer::OnInit(const Ref<RenderDevice>& device)
{
  m_RenderDevice = device;
  
  // Create framebuffers
  {
    FramebufferDesc desc;
    desc.Width = m_ViewportSize.x;
    desc.Height = m_ViewportSize.y;
    
    for (Size i = 0; i < Renderer::GetMaxFramesInFlight(); i++)
      m_Framebuffers.PushBack(m_RenderDevice->CreateFramebuffer(desc));
  }
  
  // Load Texture
  m_Texture = m_RenderDevice->LoadTexture("resources/panda.png");
  
  // Create Camera
  {
    CameraDesc desc;
    desc.Type = ProjectionType::Perspective;
    desc.Position = { 0.0f, 0.0f, 4.0f };
    desc.Rotation = { 0.0f, 0.0f, 0.0f };
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    desc.Near = 0.1f;
    desc.Far = 100.0f;
    desc.FOV = 45;
    Camera cam = Camera(desc);
    m_CameraController = CameraController(cam);
  }
  
  // Try load a model or sum
  {
    ModelDesc desc;
    desc.Path = "resources/car.obj";
    m_Model = m_RenderDevice->CreateModel(desc);
  }
}

void EditorLayer::OnTerminate()
{
}

void EditorLayer::OnUpdate()
{
  m_CameraController.OnUpdate();
  
  // Resize Framebuffer
  {
    FramebufferDesc desc = m_Framebuffers[Renderer::GetFrameNumber()]->GetDesc();
    if (desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y)
      m_Framebuffers[Renderer::GetFrameNumber()]->Resize(m_ViewportSize.x, m_ViewportSize.y);
  }
}

void EditorLayer::OnRender(const Ref<Framebuffer>& framebuffer)
{
  // Scene Pass
  {
    RenderPass scenePass;
    {
      scenePass.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
      scenePass.LoadAction = LoadAction::Clear;
      scenePass.StoreAction = StoreAction::Store;
      scenePass.Framebuffer = m_Framebuffers[Renderer::GetFrameNumber()];
    }
    RenderCommand::BeginRenderPass(scenePass);
    
    Renderer::Begin(m_CameraController.GetCamera());
    Renderer::Submit(m_Model);
    Renderer::End();
    
    RenderCommand::EndRenderPass();
  }
  
  // ImGui (Swapchain Pass)
  {
    RenderPass renderPass;
    {
      renderPass.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
      renderPass.LoadAction = LoadAction::Clear;
      renderPass.StoreAction = StoreAction::Store;
      renderPass.Framebuffer = framebuffer;
    }
    RenderCommand::BeginRenderPass(renderPass);
    
    ImGUIRenderer::Render();
    
    RenderCommand::EndRenderPass();
  }
}

void EditorLayer::OnImGuiRender()
{
  // Viewport
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
    ImGui::Begin("Viewport");
    
    bool block = !ImGui::IsWindowFocused() && !ImGui::IsWindowHovered();
    ImGUIRenderer::BlockEvents(block);
    
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)&m_Framebuffers[Renderer::GetFrameNumber()]->GetColorAttachment(), ImVec2(m_ViewportSize.x, m_ViewportSize.y));
    m_ViewportSize = { viewSize.x, viewSize.y };
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
}

void EditorLayer::OnEvent(Scope<Event>& e)
{
  
}

} // namespace Neptune

