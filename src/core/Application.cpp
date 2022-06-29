#include "neptunepch.h"
#include "Application.h"

#define NEPTUNE_ENTRYPOINT
#include "EntryPoint.h"

#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"
#include "core/Input.h"

#include "renderer/ImGUIRenderer.h"
#include "renderer/Renderer2D.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{

Application* Application::s_Application = nullptr;

Application::Application(const WindowDesc& desc)
{
  NEPTUNE_ASSERT(!s_Application, "Only one instance of application can be created!");
  s_Application = this;
  
  m_NativeApp = NativeApplication::Create();
  m_Window = Window::Create(desc);
  
  m_RenderContext = RenderContext::Create(desc.Width, desc.Height);
  m_Window->SetContext(m_RenderContext);
  
  m_RenderDevice = m_RenderContext->GetRenderDevice();
  m_Swapchain = m_RenderContext->GetSwapchain();
  
  Input::OnInit();
  
  // TODO: Move to Renderer::OnInit()
  ImGUIRenderer::OnInit(m_RenderDevice, desc.Width, desc.Height);
  Renderer2D::OnInit(m_RenderDevice);
}

Application::~Application()
{
  // TODO: Move to Renderer::OnTerminate()
  Renderer2D::OnTerminate();
  ImGUIRenderer::OnTerminate();
  
  Input::OnTerminate();
}

// shader source
const char* shaderSrc = R"(
#include <metal_stdlib>
using namespace metal;

struct VSInput
{
	float3 position [[attribute(0)]];
	float2 texCoord [[attribute(1)]];
	float4 color [[attribute(2)]];
};

struct FSInput
{
	float4 position [[position]];
	float2 texCoord;
	float4 color;
};

struct Uniform
{
	float4x4 viewProjection;
};

vertex FSInput vertexFunc(VSInput in [[stage_in]],
													constant Uniform& uniform [[buffer(1)]])
{
	FSInput out;
	out.position = uniform.viewProjection * float4(in.position, 1);
	out.texCoord = in.texCoord;
	out.color = in.color;
	return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]],
                             texture2d<float> colorTexture [[texture(0)]])
{
 	constexpr sampler textureSampler(mag_filter::linear, min_filter::linear);
 	return colorTexture.sample(textureSampler, in.texCoord) * in.color;
})";

void Application::Run()
{
  
  // Initialization
  m_Window->Show();
  
  // Create framebuffers
  {
    FramebufferDesc desc;
    desc.Width = m_ViewportSize.x;
    desc.Height = m_ViewportSize.y;
    
    // TODO: Extract in-flight frames to global
    for (Size i = 0; i < 3; i++)
    	m_Framebuffers.PushBack(m_RenderDevice->CreateFramebuffer(desc));
  }

  // Load Texture
  m_Texture = m_RenderDevice->LoadTexture("resources/panda.png");
  
  // Create Camera
  {
    CameraDesc desc;
    desc.Type = ProjectionType::Orthographic;
    desc.Position = { 0.0f, 0.0f, 3.0f };
    desc.Zoom = 0.5f;
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    desc.Near = -10.0f;
    desc.Far = 10.0f;
    m_Camera = Camera(desc);
  }
  
  constexpr Size framesInFlight = 3;
  Size frame = 0;
  DynamicArray<Ref<Fence>> fences;
  {
    FenceDesc desc;
    desc.Signaled = true;
    
    for (Size i = 0; i < framesInFlight; ++i)
      fences.PushBack(m_RenderDevice->CreateFence(desc));
  }
  
  m_Running = true;
  while (m_Running)
  {
    m_NativeApp->PollEvents();
    Input::OnUpdate(); // Begin new Input frame

    // Update In-Flight Frame Number (Don't block until rendering needs to begin)
    frame = (frame + 1) % framesInFlight;
        
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      Input::OnEvent(e);
      ImGUIRenderer::OnEvent(e);
      
      // Dispatch Events
      EventQueue::Dispatch<WindowClosedEvent>(e, [this](const WindowClosedEvent& event) {
        Stop();
        return true;
      });
      
      EventQueue::Dispatch<WindowResizedEvent>(e, [&](const WindowResizedEvent& event) {
        m_Swapchain->Resize(event.GetWidth(), event.GetHeight());
        return false;
      });
    }
    
    ImGUIRenderer::OnUpdate();
    
    // TODO: Camera Controller
    // Update Camera
    {
    	Float3 translate = Float3(0.0f);

      if (Input::KeyDown(KeyCode::KeyD)) translate.x += 0.05f;
      if (Input::KeyDown(KeyCode::KeyA)) translate.x -= 0.05f;
      if (Input::KeyDown(KeyCode::KeyW)) translate.y += 0.05f;
      if (Input::KeyDown(KeyCode::KeyS)) translate.y -= 0.05f;

      CameraDesc desc = m_Camera.GetDesc();
      desc.Position += translate;
      desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
      m_Camera.SetDesc(desc);
    }
    
    // Resize Framebuffer
    {
      FramebufferDesc desc = m_Framebuffers[frame]->GetDesc();
      if (desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y)
        m_Framebuffers[frame]->Resize(m_ViewportSize.x, m_ViewportSize.y);
    }
    
    // ImGui
    {
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
      ImGui::Begin("Viewport");
      
      ImVec2 viewSize = ImGui::GetContentRegionAvail();
      ImGui::Image((void*)&m_Framebuffers[frame]->GetColorAttachment(), ImVec2(m_ViewportSize.x, m_ViewportSize.y));
      m_ViewportSize = { viewSize.x, viewSize.y };
      
      ImGui::End();
      ImGui::PopStyleVar();
    }
    
    // Render
    m_RenderDevice->WaitForFence(fences[frame]);
    m_RenderDevice->ResetFence(fences[frame]);
    
    Ref<Framebuffer> swapchainFramebuffer = m_Swapchain->GetNextFramebuffer();
    
    CommandBuffer commandBuffer;
    RenderCommand::BeginRecording(commandBuffer);
    {
      // Scene Pass
      {
      	RenderPass scenePass;
      	{
          scenePass.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
          scenePass.LoadAction = LoadAction::Clear;
          scenePass.StoreAction = StoreAction::Store;
          scenePass.Framebuffer = m_Framebuffers[frame];
        }
        RenderCommand::BeginRenderPass(scenePass);
        
        // Renderer 2D
        Renderer2D::Begin(m_Camera);
        Renderer2D::DrawQuad(Matrix4(1.0f), Float4(1.0f));
        Renderer2D::DrawQuad(glm::translate(Matrix4(1.0f), {1.0f, 0.0f, 0.0f}), m_Texture, Float4(1.0f), 2.0f);
        Renderer2D::End();
        
        RenderCommand::EndRenderPass();
      }
    
    	// ImGui (Swapchain Pass)
      {
        RenderPass renderPass;
        {
          renderPass.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
          renderPass.LoadAction = LoadAction::Clear;
          renderPass.StoreAction = StoreAction::Store;
          renderPass.Framebuffer = swapchainFramebuffer;
        }
        RenderCommand::BeginRenderPass(renderPass);
        
        ImGUIRenderer::Render();
        
        RenderCommand::EndRenderPass();
      }
    }
  	RenderCommand::EndRecording();
    m_RenderDevice->Submit(commandBuffer, fences[frame]);
    m_Swapchain->Present(swapchainFramebuffer);
  }

  // We don't want to destroy any GPU resources that may still be required to complete rendering yet.
  m_RenderDevice->WaitIdle();
  
  // Shutdown
  m_Window->Close();
}

Application* Application::GetSingleton() noexcept
{
  return s_Application;
}

} // namespace Neptune
