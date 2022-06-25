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
const char* shaderSrc = R"(
#include <metal_stdlib>
using namespace metal;

struct VSInput
{
	float3 position [[attribute(0)]];
	float4 color [[attribute(1)]];
};

struct FSInput
{
	float4 position [[position]];
	float4 color;
};

vertex FSInput vertexFunc(VSInput in [[stage_in]])
{
	FSInput out;
	out.position = float4(in.position, 1);
	out.color = float4(in.color);
	return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]])
{
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
    desc.vertexSrc = shaderSrc;
    desc.fragmentSrc = shaderSrc;
    shader = m_RenderDevice->CreateShader(desc);
  }
  
  // Create Pipeline State
  Ref<PipelineState> pipeline;
  {
    PipelineStateDesc desc;
    desc.Shader = shader;
    desc.Layout = {
        { PipelineAttributeType::Float3, "Position" },
        { PipelineAttributeType::UChar4, "Color", true }
    };
    
    pipeline = m_RenderDevice->CreatePipelineState(desc);
  }
  
  // Create Vertex Buffer
  Ref<Buffer> vertexBuffer;
  {
    struct Vertex
    {
      Float3 position;
      UInt32 color;
    };
    
    const Vertex data[] = {
      {{-0.5f, -0.5f, 0.5f}, 0xffffffff},
      {{ 0.5f, -0.5f, 0.5f}, 0xff0000ff},
      {{ 0.5f,  0.5f, 0.5f}, 0xff00ff00},
      {{-0.5f,  0.5f, 0.5f}, 0xffff0000}
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
      // front
      0, 1, 2,
      2, 3, 0,
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
    drawCmd.Count = indexBuffer->GetSize() / sizeof(UInt16);
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
