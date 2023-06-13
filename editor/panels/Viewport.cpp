#include "neptunepch.h"
#include "Viewport.h"

#include "renderer/Renderer.h"

#include <imgui/imgui.h>

namespace Neptune
{

Viewport::Viewport(Ref<Scene> scene, Ref<RenderDevice>& device)
	: Panel(scene)
{
  // Create framebuffers
  {
    FramebufferDesc desc;
    desc.Width = m_ViewportSize.x;
    desc.Height = m_ViewportSize.y;
    
    for (Size i = 0; i < Renderer::GetMaxFramesInFlight(); i++)
      m_Framebuffers.PushBack(device->CreateFramebuffer(desc));
  }
  
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
  
  SceneRenderer::OnInit(scene);
}

Viewport::~Viewport()
{
  SceneRenderer::OnTerminate();
}

void Viewport::OnImGuiRender()
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
  ImGui::Begin("Viewport");
  
  if (ImGui::Button("Play/Stop"))
    SceneManager::ToggleRuntime();
  
  m_Focused = ImGui::IsWindowFocused();
  
  ImVec2 viewSize = ImGui::GetContentRegionAvail();
  ImGui::Image((void*)&m_Framebuffers[Renderer::GetFrameNumber()]->GetColorAttachment(), ImVec2(m_ViewportSize.x, m_ViewportSize.y));
  m_ViewportSize = { viewSize.x, viewSize.y };
  
  ImGui::End();
  ImGui::PopStyleVar();
}

void Viewport::OnUpdate(Timestep ts)
{
  if (!SceneManager::GetRuntime() && m_Focused)
  	m_CameraController.OnUpdate(ts);
  
  if (Input::KeyDown(KeyS))
  {
    SceneSerializer s(m_Scene);
    s.SaveYAML("Scene.neptscene");
  }
}

void Viewport::OnRender()
{
  // Resize framebuffer and camera if it doesn't fit
  FramebufferDesc desc = m_Framebuffers[Renderer::GetFrameNumber()]->GetDesc();
  if (desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y)
  {
    Float32 aspect = m_ViewportSize.x / m_ViewportSize.y;
    
    m_Framebuffers[Renderer::GetFrameNumber()]->Resize(m_ViewportSize.x, m_ViewportSize.y);
    
    auto desc = m_CameraController.GetCamera().GetDesc();
    desc.Aspect = aspect;
    m_CameraController.GetCamera().SetDesc(desc);
    
    SceneRenderer::OnResize(aspect);
  }
  
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
    
    if (SceneManager::GetRuntime())
      SceneRenderer::RenderRuntime();
    else
      SceneRenderer::Render(m_CameraController.GetCamera());
    
    RenderCommand::EndRenderPass();
  }
}

} // namespace Neptune
