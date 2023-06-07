#include "neptunepch.h"
#include "EditorLayer.h"

#include "panels/EntityList.h"

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
  
  // Create Panels
  {
    m_Panels.PushBack(new EntityList(&m_Scene));
    
    m_Viewport = new Viewport(&m_Scene, m_RenderDevice);
    m_Panels.PushBack(m_Viewport);
  }
  
  // ECS Test
  {
    m_Entity = m_Scene.CreateEntity("Panda");
    auto& sprite = m_Entity.AddComponent<SpriteRendererComponent>();
    sprite.Texture = m_RenderDevice->LoadTexture("resources/panda.png");
    
    struct PandaScript : public NativeScript
    {
      virtual void OnUpdate(Entity e) override
      {
      }
    };
    m_Entity.AddComponent<NativeScriptComponent>(new PandaScript());
    
    m_CameraEntity = m_Scene.CreateEntity("Scene Camera");
  }
  
  SceneManager::OnInit(&m_Scene);
}

void EditorLayer::OnTerminate()
{
  SceneManager::OnTerminate();
  
  for (auto* panel : m_Panels)
    delete panel;
}

void EditorLayer::OnUpdate()
{
  SceneManager::OnUpdate();
  
  for (auto* panel : m_Panels)
    panel->OnUpdate();
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

