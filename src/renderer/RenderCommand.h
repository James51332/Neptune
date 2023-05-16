#pragma once

#include "CommandBuffer.h"
#include "RenderPass.h"
#include "PipelineState.h"
#include "Buffer.h"
#include "Texture.h"
#include "Mesh.h"

namespace Neptune
{

class RenderDevice;

// ----- DrawCommandDesc -----------------

enum class IndexType
{
	UInt16,
  UInt32
};

enum class PrimitiveType
{
  Triangle,
  TriangleStrip,
  Line,
  LineStrip,
  Point
};

// TODO: Instancing
struct DrawCommandDesc
{
  bool Indexed = false;
  
  // Only for indexed
  Ref<Buffer> IndexBuffer;
  IndexType IndexType;
  
  // Used regardless
  PrimitiveType Type;
  Size Offset;
  Size Count;
};

// ----- RenderCommand -----------------

// This will have to be adjusted to allow for parellel encoding. For now, performance will
// be fine with sequential encoding.
class RenderCommandEncoder
{
public:
  virtual ~RenderCommandEncoder() = default;
  
  virtual void BeginRecording(CommandBuffer& commandBuffer) = 0;
  virtual void EndRecording() = 0;
  
  virtual void BeginRenderPass(const RenderPass& renderPass) = 0;
  virtual void EndRenderPass() = 0;
  
  virtual void SetPipelineState(const Ref<PipelineState>& state) = 0;
  virtual void SetVertexBuffer(const Ref<Buffer>& buffer, Size index) = 0;
  
  virtual void SetClipRect(Size x, Size y, Size w, Size h) = 0;
  
  // I don't know how other APIs handle this, but in Metal and OpenGL, we just
  // bind textures in a similiar manner to buffers and PSOs.
  virtual void BindTexture(const Ref<Texture>& texture, Size index = 0) = 0;
  
  virtual void Submit(const DrawCommandDesc& desc) = 0;
};

// ----- RenderCommand -----------------

// This is a convenience class to access RenderCommandEncoder. Every RenderDevice
// has a RenderCommandEncoder which can be manually encoded to. It is preferred to
// initialize this class and use. We may want to implement a system which allows this
// class to be used from multiple threads eventually.
class RenderCommand
{
public:
  static void OnInit(Ref<RenderDevice> device);
  static void OnTerminate();
  
  static void BeginRecording(CommandBuffer& commandBuffer);
  static void EndRecording();
  
  static void BeginRenderPass(const RenderPass& renderPass);
  static void EndRenderPass();
  
  static void SetPipelineState(const Ref<PipelineState>& state);
  static void SetVertexBuffer(const Ref<Buffer>& buffer, Size index);
  
  static void SetClipRect(Size x, Size y, Size w, Size h);
  
  static void BindTexture(const Ref<Texture>& texture, Size index = 0);
  
  static void Submit(const DrawCommandDesc& desc);
  
private:
  static Ref<RenderCommandEncoder> s_Encoder;
};

} // namespace Neptune

