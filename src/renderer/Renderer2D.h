#pragma once

#include "RenderDevice.h"
#include "Buffer.h"
#include "Texture.h"
#include "Camera.h"

namespace Neptune
{

// ----- Renderer2D -----------------

struct QuadVertex
{
  Float3 Position;
  Float4 Color;
  Float2 TexCoord;
  UInt32 TexIndex;
  Float32 TilingFactor;
};

struct Renderer2DData
{
  static constexpr Size MaxQuads = 20000;
  static constexpr Size MaxVertices = MaxQuads * 4;
  static constexpr Size MaxIndices = MaxQuads * 6;
  static constexpr Size MaxTextureSlots = 16; // TODO: Determine automatically
  
  Ref<RenderDevice> Device;
  
  Ref<Buffer> VertexBuffer;
  Ref<Buffer> IndexBuffer;
  Ref<Buffer> UniformBuffer;
  
  QuadVertex* QuadDataBase = nullptr;
  QuadVertex* QuadDataHead = nullptr;
  Size QuadIndexCount = 0;
  
  Ref<PipelineState> PipelineState;
  
  Ref<Texture> WhiteTexture;
  Ref<Texture> TextureArray[MaxTextureSlots];
  Size TextureIndex = 1; // WhiteTexture = 0
  
  Float4 QuadVertexPositions[4];
};

class Renderer2D
{
public:
  static void OnInit(const Ref<RenderDevice>& device);
  static void OnTerminate();
  
  static void Begin(const Camera& camera);
  static void End();
  
  static void DrawQuad(const Matrix4& transform, const Float4& color);
  static void DrawQuad(const Matrix4& transform, const Ref<Texture>& texture, const Float4& color = Float4(1.0f), Float32 tilingFactor = 1.0f);
  
private:
  static void StartBatch();
  static void Flush();
  
  static Renderer2DData s_Data;
};

} // namespace Neptune

