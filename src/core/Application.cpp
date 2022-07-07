#include "neptunepch.h"
#include "Application.h"

#define NEPTUNE_ENTRYPOINT
#include "EntryPoint.h"

#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"
#include "core/Input.h"

#include "renderer/Renderer.h"

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
  //m_Window = Window::Create(desc);
  //
  //m_RenderContext = RenderContext::Create(desc.Width, desc.Height);
  //m_Window->SetContext(m_RenderContext);
  //
  //m_RenderDevice = m_RenderContext->GetRenderDevice();
  //m_Swapchain = m_RenderContext->GetSwapchain();
  
  Input::OnInit();
  //Renderer::OnInit(m_RenderDevice, desc.Width, desc.Height);
}

Application::~Application()
{
  //Renderer::OnTerminate();
  Input::OnTerminate();
}

// shader source
//const char* shaderSrc = R"(
//#include <metal_stdlib>
//using namespace metal;
//
//struct VSInput
//{
//	float3 position [[attribute(0)]];
//	float2 texCoord [[attribute(1)]];
//	float4 color [[attribute(2)]];
//};
//
//struct FSInput
//{
//	float4 position [[position]];
//	float2 texCoord;
//	float4 color;
//};
//
//struct Uniform
//{
//	float4x4 viewProjection;
//};
//
//vertex FSInput vertexFunc(VSInput in [[stage_in]],
//													constant Uniform& uniform [[buffer(1)]])
//{
//	FSInput out;
//	out.position = uniform.viewProjection * float4(in.position, 1);
//	out.texCoord = in.texCoord;
//	out.color = in.color;
//	return out;
//}
//
//fragment float4 fragmentFunc(FSInput in [[stage_in]],
//                             texture2d<float> colorTexture [[texture(0)]])
//{
// 	constexpr sampler textureSampler(mag_filter::linear, min_filter::linear);
// 	return colorTexture.sample(textureSampler, in.texCoord) * in.color;
//})";

void Application::Run()
{
  // Initialization
  //m_Window->Show();
  
  //DynamicArray<Ref<Fence>> fences;
  //{
  //  FenceDesc desc;
  //  desc.Signaled = true;
  //  
  //  for (Size i = 0; i < Renderer::GetMaxFramesInFlight(); ++i)
  //    fences.PushBack(m_RenderDevice->CreateFence(desc));
  //}
  
  // Initialize Layers
  for (auto layer : m_LayerStack)
    layer->OnInit(m_RenderDevice);
  
  m_Running = true;
  while (m_Running)
  {
    m_NativeApp->PollEvents();
    Input::OnUpdate(); // Begin new Input frame
    
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      Input::OnEvent(e);
      //Renderer::OnEvent(e);
      
      // Dispatch Events
      EventQueue::Dispatch<WindowClosedEvent>(e, [this](const WindowClosedEvent& event) {
        Stop();
        return true;
      });
      
      EventQueue::Dispatch<WindowResizedEvent>(e, [&](const WindowResizedEvent& event) {
        m_Swapchain->Resize(event.GetWidth(), event.GetHeight());
        return false;
      });
      
      // Send remaining to layers
      for (auto it = m_LayerStack.rbegin(); it < m_LayerStack.rend(); ++it)
        if (!e->Handled)
          (*it)->OnEvent(e);
    }
    
    //Renderer::OnUpdate();
    
    for (auto& layer : m_LayerStack)
      layer->OnUpdate();
    
    //for (auto& layer : m_LayerStack)
    //  layer->OnImGuiRender();
    
    // Render
   /* m_RenderDevice->WaitForFence(fences[Renderer::GetFrameNumber()]);
    m_RenderDevice->ResetFence(fences[Renderer::GetFrameNumber()]);
    
    Ref<Framebuffer> swapchainFramebuffer = m_Swapchain->GetNextFramebuffer();
    
    CommandBuffer commandBuffer;
    RenderCommand::BeginRecording(commandBuffer);
    {
      for (auto& layer : m_LayerStack)
        layer->OnRender(swapchainFramebuffer);
    }
  	RenderCommand::EndRecording();
    m_RenderDevice->Submit(commandBuffer, fences[Renderer::GetFrameNumber()]);
    m_Swapchain->Present(swapchainFramebuffer);*/
  }

  // We don't want to destroy any GPU resources that may still be required to complete rendering yet.
  //m_RenderDevice->WaitIdle();
  
  for (auto& layer : m_LayerStack)
    layer->OnTerminate();
  
  // Shutdown
  m_Window->Close();
}

Application* Application::GetSingleton() noexcept
{
  return s_Application;
}

} // namespace Neptune
