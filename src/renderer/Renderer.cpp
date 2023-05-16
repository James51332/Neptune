#include "neptunepch.h"
#include "Renderer.h"

#include "renderer/Renderer2D.h"
#include "renderer/ImGUIRenderer.h"

const char* meshSrc = R"(
#include <metal_stdlib>
using namespace metal;

struct VSInput
{
  float3 position [[attribute(0)]];
  float3 normal [[attribute(1)]];
  float4 color [[attribute(2)]];
  float2 uv [[attribute(3)]];
};

struct FSInput
{
  float4 position [[position]];
	float3 normal;
  float4 color;
	float2 uv;
};

struct Uniform
{
  float4x4 viewProjection;
};

vertex FSInput vertexFunc(VSInput in [[stage_in]],
                        constant Uniform& uniform [[buffer(1)]])
{
  float3 envLightDir(1, 1, 0);

  FSInput out;
  out.position = uniform.viewProjection * float4(in.position , 1);
  out.color = in.color * float4(float3(max(dot(abs(in.normal), envLightDir), 0.0f)), 1);
  return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]])
{
  return in.color;
})";

namespace Neptune
{

RenderAPI Renderer::s_API = RenderAPI::Metal; // TODO: Expose this to Application
Size Renderer::s_Frame = 0;

bool Renderer::s_Recording = false;
Camera Renderer::s_SceneCamera;
Ref<Buffer> Renderer::s_UniformBuffer;
Ref<PipelineState> Renderer::s_MeshPipeline;

void Renderer::OnInit(Ref<RenderDevice>& device, Size width, Size height)
{
  ImGUIRenderer::OnInit(device, width, height);
  Renderer2D::OnInit(device);
  
  // Create Uniform Buffer
  {
    BufferDesc ubDesc;
    ubDesc.Type = BufferType::Uniform;
    ubDesc.Usage = BufferUsage::Dynamic;
    ubDesc.Size = sizeof(Matrix4);
    ubDesc.Data = nullptr;
    s_UniformBuffer = device->CreateBuffer(ubDesc);
  }
  
  // Create Mesh Pipeline
  {
    PipelineStateDesc desc;
    
    desc.Layout = {
      { PipelineAttributeType::Float3, "Position" },
      { PipelineAttributeType::Float3, "Normal" },
      { PipelineAttributeType::Float4, "Color" },
      { PipelineAttributeType::Float2, "UV" }
    };
      
    DepthStencilState dss;
    dss.DepthWriteEnabled = true;
    dss.Function = CompareFunction::Less;
    desc.DepthStencilState = dss;
      
    ShaderDesc s;
    s.vertexSrc = meshSrc;
    s.fragmentSrc = meshSrc;
    desc.Shader = device->CreateShader(s);
      
    s_MeshPipeline = device->CreatePipelineState(desc);
  }
}

void Renderer::OnUpdate()
{
  s_Frame = (s_Frame + 1) % s_FramesInFlight;
}

void Renderer::OnTerminate()
{
  Renderer2D::OnTerminate();
  ImGUIRenderer::OnTerminate();
}

void Renderer::OnEvent(Scope<Event> &event)
{
  ImGUIRenderer::OnEvent(event);
}

RenderAPI Renderer::GetAPI() noexcept
{
  return s_API;
}

Size Renderer::GetMaxFramesInFlight() noexcept
{
  return s_FramesInFlight;
}

Size Renderer::GetFrameNumber() noexcept
{
  return s_Frame;
}

void Renderer::Begin(const Camera &camera)
{
  NEPTUNE_ASSERT(!s_Recording, "Renderer has already begun!");
  s_Recording = true;
  
  s_SceneCamera = camera;
  s_UniformBuffer->Update(sizeof(Matrix4), &s_SceneCamera.GetViewProjectionMatrix()[0][0]);
}

void Renderer::End()
{
  NEPTUNE_ASSERT(s_Recording, "Renderer has already ended!");
  s_Recording = false;
}

void Renderer::Submit(const Ref<Mesh> &mesh, const Ref<Material> &material)
{
  NEPTUNE_ASSERT(s_Recording, "Renderer is not recording!");

  RenderCommand::SetVertexBuffer(mesh->m_VertexBuffer, 0);
  RenderCommand::SetVertexBuffer(s_UniformBuffer, 1);
  RenderCommand::SetPipelineState(s_MeshPipeline);
  
  DrawCommandDesc cmd;
  cmd.Type = PrimitiveType::Triangle;
  cmd.IndexBuffer = mesh->m_IndexBuffer;
  cmd.Indexed = true;
  cmd.IndexType = IndexType::UInt32;
  cmd.Count = mesh->m_IndexBuffer->GetSize() / sizeof(MeshIndex);
  cmd.Offset = 0;
  RenderCommand::Submit(cmd);
}

} // namespace Neptune
