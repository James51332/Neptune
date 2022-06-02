#include "neptunepch.h"
#include "Application.h"

#define NEPTUNE_ENTRYPOINT
#include "EntryPoint.h"

#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"

#include "renderer/RenderContext.h"
#include "renderer/RenderCommand.h"
#include "renderer/PipelineState.h"
#include "renderer/RenderPass.h"
#include "renderer/CommandBuffer.h"
#include "renderer/Shader.h"

namespace Neptune
{

Application* Application::s_Application = nullptr;

Application::Application(const WindowDesc& desc)
{
  NEPTUNE_ASSERT(!s_Application, "Only one instance of application can be created!");
  s_Application = this;
  
  m_NativeApp = NativeApplication::Create();
  m_Window = Window::Create(desc);
  
  m_RenderContext = RenderContext::Create();
  m_Window->SetContext(m_RenderContext);
}

Application::~Application()
{
  
}

// shader source
const char* vertex = R"(
#include <metal_stdlib>
using namespace metal;

constant float3 vertices[] =
{
float3(0.0f, 0.5f, 0.0f),
float3(0.5f, -0.5f, 0.0f),
float3(-0.5f, -0.5f, 0.0f)
};

vertex float4 vertexFunc(uint vertexID [[vertex_id]])
{
return float4(vertices[vertexID], 1);
})";

const char* fragment = R"(
fragment float4 fragmentFunc(float4 in [[stage_in]])
{
// Return the interpolated color.
return float4(1.0, 1.0, 1.0, 1.0);
})";


void Application::Run()
{
  
  // Initialization
  m_Window->Show();
  
  // Create Shader
  Ref<Shader> shader;
  {
    ShaderDesc desc;
    desc.vertexSrc = vertex;
    desc.fragmentSrc = fragment;
    shader = m_RenderContext->GetRenderDevice()->CreateShader(desc);
  }
  
  // Create Pipeline State
  Ref<PipelineState> pipeline;
  {
    PipelineStateDesc desc;
    desc.Shader = shader;
    pipeline = m_RenderContext->GetRenderDevice()->CreatePipelineState(desc);
  }
  
  m_Running = true;
  while (m_Running)
  {
    m_NativeApp->PollEvents();
    
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      // Dispatch Events
      m_EventQueue.Dispatch<WindowClosedEvent>(e, [this](const WindowClosedEvent& event) {
        Stop();
        return true;
      });
    } 
    
    // Render
    {
      Ref<RenderDevice> device = m_RenderContext->GetRenderDevice();
      Ref<Swapchain> swapchain = m_RenderContext->GetSwapchain();
      Ref<Framebuffer> framebuffer = swapchain->GetNextFramebuffer();
      
      CommandBuffer commandBuffer;
      RenderCommand::BeginRecording(commandBuffer);
      {
        RenderPass renderPass;
        {
          renderPass.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
          renderPass.LoadAction = LoadAction::Clear;
          renderPass.Framebuffer = framebuffer;
        }
        RenderCommand::BeginRenderPass(renderPass);
        
        RenderCommand::SetPipelineState(pipeline);
        RenderCommand::DrawTriangles(0, 3);
        
        RenderCommand::EndRenderPass();
      }
      RenderCommand::EndRecording();
      
      device->Submit(commandBuffer);
      swapchain->Present(framebuffer);
    }
  }
  
  // Shutdown
  m_Window->Close();
}

Application* Application::GetSingleton() noexcept
{
  return s_Application;
}

} // namespace Neptune
