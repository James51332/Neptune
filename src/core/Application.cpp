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
  Renderer::OnInit(m_RenderDevice, desc.Width, desc.Height);
}

Application::~Application()
{
  Renderer::OnTerminate();
  Input::OnTerminate();
}

void Application::Run()
{
  // Initialization
  m_Window->Show();
  
  DynamicArray<Ref<Fence>> fences;
  {
    FenceDesc desc;
    desc.Signaled = true;
    
    for (Size i = 0; i < Renderer::GetMaxFramesInFlight(); ++i)
      fences.PushBack(m_RenderDevice->CreateFence(desc));
  }
  
  // Initialize Layers
  for (auto layer : m_LayerStack)
    layer->OnInit(m_RenderDevice);
  
  m_Running = true;
  m_LastTime = m_NativeApp->PollTime();
  while (m_Running)
  {
    m_NativeApp->PollEvents();
    Input::OnUpdate(); // Begin new Input frame
    
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      Renderer::OnEvent(e);
      Input::OnEvent(e);
      
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
    
    Renderer::OnUpdate();
    
    Float32 time = m_NativeApp->PollTime();
    Timestep ts = time - m_LastTime;
    m_LastTime = time;
    
    for (auto& layer : m_LayerStack)
      layer->OnUpdate(ts);
    
    for (auto& layer : m_LayerStack)
      layer->OnImGuiRender();
    
    // Render
    m_RenderDevice->WaitForFence(fences[Renderer::GetFrameNumber()]);
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
    m_Swapchain->Present(swapchainFramebuffer);
  }

  // We don't want to destroy any GPU resources that may still be required to complete rendering yet.
  m_RenderDevice->WaitIdle();
  
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
