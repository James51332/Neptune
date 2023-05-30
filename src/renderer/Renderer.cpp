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

struct Light
{
  float3 lightPos;
  float4 lightColor;
};

struct FSInput
{
  float4 position [[position]];
	float3 worldPos;
	float3 camPos;
	float3 normal;
  float4 color;
	float2 uv;
	Light light;
};

struct WorldUniform
{
  float4x4 viewProjection;
	float3 camPosition;
};

struct ObjectUniform
{
	float4x4 model;
};

vertex FSInput vertexFunc(VSInput in [[stage_in]],
                        	constant WorldUniform& uniform [[buffer(1)]],
													constant Light& light [[buffer(2)]],
													constant ObjectUniform& object [[buffer(3)]])
{

  FSInput out;
  out.position = uniform.viewProjection * object.model * float4(in.position, 1);
  out.worldPos = in.position;
	out.camPos = uniform.camPosition;
  out.normal = in.normal;
  out.color = in.color;
  out.light = light;
  return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]])
{
  float ambientStrength = 0.1f;
  float3 ambient = ambientStrength * float3(in.light.lightColor);

	float3 norm = normalize(in.normal);
	float3 lightDir = normalize(in.light.lightPos - in.worldPos);

	float diff = max(dot(norm, lightDir), 0.0);
	float3 diffuse = float3(diff * in.light.lightColor);

	float specularStrength = 1.0f;
 	float3 viewDir = normalize(in.camPos - in.worldPos);
	float3 reflectDir = -reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float3 specular = specularStrength * spec * float3(in.light.lightColor);

  float3 result = (ambient + diffuse + specular) * float3(in.color);
  return float4(result, 1.0);
})";

namespace Neptune
{

RenderAPI Renderer::s_API = RenderAPI::Metal; // TODO: Expose this to Application
Size Renderer::s_Frame = 0;

bool Renderer::s_Recording = false;
Camera Renderer::s_SceneCamera;
Ref<Buffer> Renderer::s_UniformBuffer;
Ref<Buffer> Renderer::s_LightUniformBuffer;
Ref<PipelineState> Renderer::s_MeshPipeline;
Ref<Buffer> Renderer::s_ModelUniformBuffer;
Float3 Renderer::s_LightPos;

void Renderer::OnInit(Ref<RenderDevice>& device, Size width, Size height)
{
  ImGUIRenderer::OnInit(device, width, height);
  Renderer2D::OnInit(device);
  
  // Create Uniform Buffer
  {
    BufferDesc ubDesc;
    ubDesc.Type = BufferType::Uniform;
    ubDesc.Usage = BufferUsage::Dynamic;
    ubDesc.Size = sizeof(Matrix4) + sizeof(Float4);
    ubDesc.Data = nullptr;
    s_UniformBuffer = device->CreateBuffer(ubDesc);
  }
  
  // Create Light Uniform Buffer
  {
    BufferDesc ubDesc;
    ubDesc.Type = BufferType::Uniform;
    ubDesc.Usage = BufferUsage::Dynamic;
    ubDesc.Size = 32;
    ubDesc.Data = nullptr;
    s_LightUniformBuffer = device->CreateBuffer(ubDesc);
  }
  
  // Create Model Uniform Buffer
  {
    BufferDesc ubDesc;
    ubDesc.Type = BufferType::Uniform;
    ubDesc.Usage = BufferUsage::Dynamic;
    ubDesc.Size = sizeof(Matrix4);
    ubDesc.Data = nullptr;
    s_ModelUniformBuffer = device->CreateBuffer(ubDesc);
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

struct WorldUniform
{
  Matrix4 matrix;
  Float3 pos;
};

void Renderer::Begin(const Camera &camera)
{
  NEPTUNE_ASSERT(!s_Recording, "Renderer has already begun!");
  s_Recording = true;
  
  s_SceneCamera = camera;
  WorldUniform uniform;
  uniform.matrix = s_SceneCamera.GetViewProjectionMatrix();
  uniform.pos = s_SceneCamera.GetDesc().Position;
  
  s_UniformBuffer->Update(sizeof(Matrix4), &uniform);
}

void Renderer::End()
{
  NEPTUNE_ASSERT(s_Recording, "Renderer has already ended!");
  s_Recording = false;
}

struct Light
{
  Float3 Position;
  Float4 Color;
};

void Renderer::SetLight(const Float3 &lightPos, const Float4& color)
{
  s_LightPos = lightPos;
  
  Light light;
  light.Position = lightPos;
  light.Color = color;
  s_LightUniformBuffer->Update(sizeof(Light), &light);
}

void Renderer::Submit(const Ref<Mesh> &mesh, const Ref<Material> &material, const Matrix4& transform)
{
  NEPTUNE_ASSERT(s_Recording, "Renderer is not recording!");

  RenderCommand::SetVertexBuffer(mesh->m_VertexBuffer, 0);
  RenderCommand::SetVertexBuffer(s_UniformBuffer, 1);
  RenderCommand::SetVertexBuffer(s_LightUniformBuffer, 2);
  
  s_ModelUniformBuffer->Update(sizeof(Matrix4), &transform[0][0]);
  RenderCommand::SetVertexBuffer(s_ModelUniformBuffer, 3);
  
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

void Renderer::Submit(const Ref<Model> &model, const Matrix4& transform)
{
  for (auto mesh : model->m_Meshes)
  {
    Submit(mesh, model->m_Materials[0], transform);
  }
}

} // namespace Neptune
