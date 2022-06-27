#include "neptunepch.h"
#include "Application.h"

#define NEPTUNE_ENTRYPOINT
#include "EntryPoint.h"

#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"
#include "core/Input.h"

#include "renderer/RenderContext.h"
#include "renderer/RenderCommand.h"
#include "renderer/PipelineState.h"
#include "renderer/RenderPass.h"
#include "renderer/CommandBuffer.h"
#include "renderer/Shader.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"

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
}

Application::~Application()
{
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
      { PipelineAttributeType::Float2, "TexCoord" },
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
      Float2 texCoord;
      UInt32 color = 0xffffffff;
    };
    
    const Vertex data[] = {
      {{-0.5f,0.5f,-0.5f},  {0.0f, 0.0f}},
      {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
      {{0.5f,-0.5f,-0.5f},  {1.0f, 1.0f}},
      {{0.5f,0.5f,-0.5f},   {1.0f, 0.0f}},
      {{-0.5f,0.5f,0.5f},   {0.0f, 0.0f}},
      {{-0.5f,-0.5f,0.5f},  {0.0f, 1.0f}},
      {{0.5f,-0.5f,0.5f},   {1.0f, 1.0f}},
      {{0.5f,0.5f,0.5f},    {1.0f, 0.0f}},
      {{0.5f,0.5f,-0.5f},   {0.0f, 0.0f}},
      {{0.5f,-0.5f,-0.5f},  {0.0f, 1.0f}},
      {{0.5f,-0.5f,0.5f},   {1.0f, 1.0f}},
      {{0.5f,0.5f,0.5f},    {1.0f, 0.0f}},
      {{-0.5f,0.5f,-0.5f},  {0.0f, 0.0f}},
      {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
      {{-0.5f,-0.5f,0.5f},  {1.0f, 1.0f}},
      {{-0.5f,0.5f,0.5f},   {1.0f, 0.0f}},
      {{-0.5f,0.5f,0.5f},   {0.0f, 0.0f}},
      {{-0.5f,0.5f,-0.5f},  {0.0f, 1.0f}},
      {{0.5f,0.5f,-0.5f},   {1.0f, 1.0f}},
      {{0.5f,0.5f,0.5f},    {1.0f, 0.0f}},
      {{-0.5f,-0.5f,0.5f},  {0.0f, 0.0f}},
      {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
      {{0.5f,-0.5f,-0.5f},  {1.0f, 1.0f}},
      {{0.5f,-0.5f,0.5f},   {1.0f, 0.0f}}
    };
    
    BufferDesc desc;
    desc.Type = BufferType::Vertex;
    desc.Usage = BufferUsage::Static;
    desc.Size = sizeof(data);
    desc.Data = (void*)data;
    vertexBuffer = m_RenderDevice->CreateBuffer(desc);
  }

  // Create Index Buffer
  Ref<Buffer> indexBuffer;
  {
    const UInt16 data[] = {
      0,1,3,
      3,1,2,
      4,5,7,
      7,5,6,
      8,9,11,
      11,9,10,
      12,13,15,
      15,13,14,
      16,17,19,
      19,17,18,
      20,21,23,
      23,21,22
    };
    
    BufferDesc desc;
    desc.Type = BufferType::Index;
    desc.Usage = BufferUsage::Static;
    desc.Size = sizeof(data);
    desc.Data = (void*)data;
    indexBuffer = m_RenderDevice->CreateBuffer(desc);
  }
  
  // Load Texture
  Ref<Texture> texture;
  {
    texture = m_RenderDevice->LoadTexture("resources/panda.png");
  }
  
  // Create Camera
  Camera camera;
  {
    CameraDesc desc;
    desc.Type = ProjectionType::Perspective;
    desc.Position = { 1.0f, 1.0f, 2.0f };
    desc.Width = 4.0f;
    desc.Height = 4.0f;
    camera = Camera(desc);
  }
  
  // Create Uniform Buffer
  Ref<Buffer> uniformBuffer;
  {
    BufferDesc desc;
    desc.Type = BufferType::Uniform;
    desc.Usage = BufferUsage::Dynamic;
    desc.Size = sizeof(Matrix4);
    desc.Data = (void*)&camera.GetViewProjectionMatrix()[0][0];
    uniformBuffer = m_RenderDevice->CreateBuffer(desc);
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
    Input::OnUpdate(); // Begin new Input frame
    
    // Event Stage
    Scope<Event> e;
    while (m_EventQueue.PopEvent(e))
    {
      Input::OnEvent(e);
      
      // Dispatch Events
      EventQueue::Dispatch<WindowClosedEvent>(e, [this](const WindowClosedEvent& event) {
        Stop();
        return true;
      });
      
      EventQueue::Dispatch<WindowResizedEvent>(e, [=](const WindowResizedEvent& event) {
        m_Swapchain->Resize(event.GetWidth(), event.GetHeight());
        return false;
      });
    }
    
    // Camera Movement
    {
    	Float3 translate = Float3(0.0f);
      
      if (Input::KeyDown(KeyCode::KeyD)) translate.x += 0.05f;
      if (Input::KeyDown(KeyCode::KeyA)) translate.x -= 0.05f;
      if (Input::KeyDown(KeyCode::KeyW)) translate.y += 0.05f;
      if (Input::KeyDown(KeyCode::KeyS)) translate.y -= 0.05f;
      
      CameraDesc desc = camera.GetDesc();
      desc.Position += translate;
      camera.SetDesc(desc);
      uniformBuffer->Update(sizeof(Matrix4), &camera.GetViewProjectionMatrix()[0][0]);
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
        RenderCommand::SetVertexBuffer(uniformBuffer, 1);
        RenderCommand::SetPipelineState(pipeline);
        RenderCommand::BindTexture(texture, 0);
        
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
