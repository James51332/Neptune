#include "neptunepch.h"
#include "EditorLayer.h"

#include "panels/EntityList.h"
#include "panels/Inspector.h"

#include <imgui/imgui.h>

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
  
  m_Scene = CreateRef<Scene>();
  
  SceneSerializer serializer(m_Scene);
  serializer.LoadYAML("Scene.neptscene");
  
  SceneManager::OnInit(m_Scene);
  
  // Create Panels
  {
    m_Panels.PushBack(new EntityList(m_Scene));
    
    m_Viewport = new Viewport(m_Scene, m_RenderDevice);
    m_Panels.PushBack(m_Viewport);
    m_Panels.PushBack(new Inspector(m_Scene));
  }
}

void EditorLayer::OnTerminate()
{
  SceneManager::OnTerminate();
  
  for (auto* panel : m_Panels)
    delete panel;
}

void EditorLayer::OnUpdate(Timestep ts)
{
  SceneManager::OnUpdate(ts);
  
  for (auto* panel : m_Panels)
    panel->OnUpdate(ts);
}

void EditorLayer::OnRender(const Ref<Framebuffer>& framebuffer)
{
  m_Viewport->OnRender();
  
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
  for (auto* panel : m_Panels)
    panel->OnImGuiRender();
}

void EditorLayer::OnEvent(Scope<Event>& e)
{
  for (auto* panel : m_Panels)
    panel->OnEvent(e);
}

} // namespace Neptune

