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
  
  m_RenderDevice = m_RenderContext->GetRenderDevice();
  m_Swapchain = m_RenderContext->GetSwapchain();
}

Application::~Application()
{
  
}

// shader source
const char* vertex = R"(
#include <metal_stdlib>
using namespace metal;

struct Out
{
	float4 position [[position]];
	float4 color;
};

vertex Out vertexFunc(const device packed_float3* vertices [[buffer(0)]],
                         uint vertexID [[vertex_id]])
{
	Out out;
	out.position = float4(vertices[vertexID], 1);
	out.color = float4(2 * vertices[vertexID].xy, 1, 1);
	return out;
})";

const char* fragment = R"(
#include <metal_stdlib>
using namespace metal;

struct Out
{
  float4 position [[position]];
  float4 color;
};

fragment float4 fragmentFunc(Out in [[stage_in]])
{
	// Return the interpolated color.
	return in.color;
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
    shader = m_RenderDevice->CreateShader(desc);
  }
  
  // Create Pipeline State
  Ref<PipelineState> pipeline;
  {
    PipelineStateDesc desc;
    desc.Shader = shader;
    pipeline = m_RenderDevice->CreatePipelineState(desc);
  }
  
  // Create Vertex Buffer
  Ref<Buffer> vertexBuffer;
  {
    const Float32 data[] = {
      0.0f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f, -0.5f, 0.0f
    };
    
    BufferDesc desc;
    desc.Type = BufferType::Vertex;
    desc.Usage = BufferUsage::Static;
    desc.Size = sizeof(data);
    desc.Data = (void*)data;
    vertexBuffer = m_RenderDevice->CreateBuffer(desc);
  }
  
  // Create index buffer
  Ref<Buffer> indexBuffer;
  {
    const UInt16 data[] = {
    	0, 1, 2
    };
    
    BufferDesc desc;
    desc.Type = BufferType::Index;
    desc.Usage = BufferUsage::Static;
    desc.Size = sizeof(data);
    desc.Data = (void*)data;
    indexBuffer = m_RenderDevice->CreateBuffer(desc);
  }
  
  // Create draw command desc (can be reused, since the same for each call)
  DrawCommandDesc drawCmd;
  {
    drawCmd.Indexed = true;
    drawCmd.IndexBuffer = indexBuffer;
    drawCmd.IndexType = IndexType::UInt16;
    
    drawCmd.Type = PrimitiveType::Triangle;
    drawCmd.Offset = 0;
    drawCmd.Count = 3;
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
      Ref<Framebuffer> framebuffer = m_Swapchain->GetNextFramebuffer();
      
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
        
        RenderCommand::SetVertexBuffer(vertexBuffer, 0);
        RenderCommand::SetPipelineState(pipeline);
        
        RenderCommand::Submit(drawCmd);
        
        RenderCommand::EndRenderPass();
      }
      RenderCommand::EndRecording();
      
      m_RenderDevice->Submit(commandBuffer);
      m_Swapchain->Present(framebuffer);
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
