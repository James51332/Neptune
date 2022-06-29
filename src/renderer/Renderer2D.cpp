#include "neptunepch.h"
#include "Renderer2D.h"

namespace Neptune
{

// ----- Shader Source -----------------

static const char* Renderer2DShaderSource = R"(
#include <metal_stdlib>
using namespace metal;

struct VSInput
{
  float3 position [[attribute(0)]];
  float4 color [[attribute(1)]];
  float2 texCoord [[attribute(2)]];
	uint texIndex [[attribute(3)]];
	float tilingFactor [[attribute(4)]];
};

struct FSInput
{
  float4 position [[position]];
  float4 color;
  float2 texCoord;
	uint texIndex;
	float tilingFactor;
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
  out.color = in.color;
  out.texCoord = in.texCoord;
	out.texIndex = in.texIndex;
	out.tilingFactor = in.tilingFactor;
  return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]],
                             array<texture2d<float>, 16> texture [[texture(0)]])
{
  constexpr sampler textureSampler(address::repeat, mag_filter::linear, min_filter::linear);
  float4 texColor = texture[in.texIndex].sample(textureSampler, in.texCoord * in.tilingFactor);
  return in.color * texColor;
})";


// ----- Renderer2D -----------------

Renderer2DData Renderer2D::s_Data;

void Renderer2D::OnInit(const Ref<RenderDevice>& device)
{
  s_Data.Device = device;
  
  // Create Vertex Buffer
  {
    BufferDesc desc;
    desc.Type = BufferType::Vertex;
    desc.Usage = BufferUsage::Dynamic;
    desc.Size = s_Data.MaxVertices * sizeof(QuadVertex);
    desc.Data = nullptr;
    s_Data.VertexBuffer = s_Data.Device->CreateBuffer(desc);
    
    s_Data.QuadDataBase = new QuadVertex[s_Data.MaxVertices];
    s_Data.QuadDataHead = s_Data.QuadDataBase;
  }
  
  // Create Index Buffer
  {
    BufferDesc desc;
    desc.Type = BufferType::Index;
    desc.Usage = BufferUsage::Static;
    desc.Size = s_Data.MaxIndices * sizeof(QuadVertex);
    
    // Generate Indices
    UInt32* quadIndices = new UInt32[s_Data.MaxIndices];
    {
      UInt32 offset = 0; // transforms are baked into data so we need to offset indices to vertices later in data
      for (Size i = 0; i < s_Data.MaxIndices; i += 6)
      {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;
        
        quadIndices[i + 3] = offset + 0;
        quadIndices[i + 4] = offset + 2;
        quadIndices[i + 5] = offset + 3;
        
        offset += 4; // increase four vertices for every six indices
      }
      
      desc.Data = quadIndices;
    }
    
    s_Data.IndexBuffer = s_Data.Device->CreateBuffer(desc);
    
    // Delete Indices
    delete[] quadIndices;
  }
  
  // Create Uniform Buffer
  {
    BufferDesc desc;
    desc.Type = BufferType::Uniform;
    desc.Usage = BufferUsage::Dynamic;
    desc.Size = sizeof(Matrix4);
    desc.Data = nullptr;
    s_Data.UniformBuffer = s_Data.Device->CreateBuffer(desc);
  }
  
  // Create Pipeline State
  {
    PipelineStateDesc desc;
    
    // Shader
    Ref<Shader> shader;
    {
      ShaderDesc shaderDesc;
      shaderDesc.vertexSrc = Renderer2DShaderSource;
      shaderDesc.fragmentSrc = Renderer2DShaderSource;
      shader = s_Data.Device->CreateShader(shaderDesc);
    }
    desc.Shader = shader;
    
    // Layout
    desc.Layout = {
      { PipelineAttributeType::Float3, "Position" },
      { PipelineAttributeType::Float4, "Color" },
      { PipelineAttributeType::Float2, "TexCoord" },
      { PipelineAttributeType::UInt, "TexIndex" },
      { PipelineAttributeType::Float, "TilingFactor" }
    };
    
    // Depth
    DepthStencilState depthStencilState;
    {
      depthStencilState.DepthWriteEnabled = false;
      depthStencilState.Function = CompareFunction::Always;
    }
    desc.DepthStencilState = depthStencilState;
    
    s_Data.PipelineState = s_Data.Device->CreatePipelineState(desc);
  }
  
  // Create White Texture
  {
    TextureDesc desc;
    desc.Type = TextureType::Texture2D;
    desc.Width = 1;
    desc.Height = 1;
    desc.Mipmapped = false;
    desc.PixelFormat = PixelFormat::RGBA8Unorm;
    desc.RenderTarget = false;
    
    UInt32 data = 0xffffffff;
    desc.Data = (void*)&data;
    
    s_Data.WhiteTexture = s_Data.Device->CreateTexture(desc);
    s_Data.TextureArray[0] = s_Data.WhiteTexture;
  }
  
  // Create Quad Vertices
  {
    s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    s_Data.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
  }
}

void Renderer2D::OnTerminate()
{
  // Free Quad Data
  delete[] s_Data.QuadDataBase;
}

void Renderer2D::Begin(const Camera &camera)
{
  s_Data.UniformBuffer->Update(sizeof(Matrix4), &camera.GetViewProjectionMatrix()[0][0]);
  StartBatch();
}

void Renderer2D::End()
{
  Flush();
}

void Renderer2D::StartBatch()
{
  s_Data.QuadDataHead = s_Data.QuadDataBase;
  s_Data.QuadIndexCount = 0;
  s_Data.TextureIndex = 1;
}

void Renderer2D::Flush()
{
  if (s_Data.QuadIndexCount == 0) return;
  
  Size dataSize = static_cast<Size>(s_Data.QuadDataHead - s_Data.QuadDataBase) * sizeof(QuadVertex);
  s_Data.VertexBuffer->Update(dataSize, s_Data.QuadDataBase);
  
  RenderCommand::SetVertexBuffer(s_Data.VertexBuffer, 0);
  RenderCommand::SetVertexBuffer(s_Data.UniformBuffer, 1);
  RenderCommand::SetPipelineState(s_Data.PipelineState);
  
  for (Size i = 0; i < s_Data.TextureIndex; i++)
    RenderCommand::BindTexture(s_Data.TextureArray[i], i);
  
  DrawCommandDesc cmd;
  {
    cmd.Type = PrimitiveType::Triangle;
    cmd.Indexed = true;
    cmd.IndexBuffer = s_Data.IndexBuffer;
    cmd.IndexType = IndexType::UInt32;
    cmd.Offset = 0;
    cmd.Count = s_Data.QuadIndexCount;
  }
  RenderCommand::Submit(cmd);
}

void Renderer2D::DrawQuad(const Matrix4 &transform, const Float4 &color)
{
  DrawQuad(transform, s_Data.WhiteTexture, color);
}

void Renderer2D::DrawQuad(const Matrix4 &transform, const Ref<Texture> &texture, const Float4& color, Float32 tilingFactor)
{
  if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
  {
    Flush();
    StartBatch();
  }
  
  Size texIndex = s_Data.MaxTextureSlots; // start at max (actual index if found)
  for (Size i = 0; i < s_Data.MaxTextureSlots; ++i)
  {
    if (s_Data.TextureArray[i].Raw() == texture.Raw())
    {
      texIndex = i;
      s_Data.TextureIndex = (i + 1) > s_Data.TextureIndex ? (i + 1) : s_Data.TextureIndex; // To ensure the texture is bound, we take the max
      break;
    }
  }
  
  if (texIndex == s_Data.MaxTextureSlots) // texture not found in array
  {
    if (s_Data.TextureIndex >= s_Data.MaxTextureSlots) // next slot is out of bound
    {
      Flush();
      StartBatch();
    }
    
    texIndex = s_Data.TextureIndex;
    s_Data.TextureArray[texIndex] = texture;
    s_Data.TextureIndex++;
  }
  
  constexpr Float2 TextureCoords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
  constexpr Size QuadVertexCount = 4;
  for (Size i = 0; i < QuadVertexCount; i++)
  {
    s_Data.QuadDataHead->Position = transform * s_Data.QuadVertexPositions[i];
    s_Data.QuadDataHead->Color = color;
    s_Data.QuadDataHead->TexCoord = TextureCoords[i];
    s_Data.QuadDataHead->TexIndex = static_cast<UInt32>(texIndex);
    s_Data.QuadDataHead->TilingFactor = tilingFactor;
    s_Data.QuadDataHead++; // On the next iteration/draw we will use the next vertex
  }
  
  s_Data.QuadIndexCount += 6;
}

} // namespace Neptune
