#pragma once

#include "CommandBuffer.h"
#include "RenderPass.h"
#include "PipelineState.h"

namespace Neptune
{

class RenderDevice;

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
  
  virtual void DrawTriangles(Size start, Size count) = 0;
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
  
  static void DrawTriangles(Size start, Size count);
  
private:
  static Ref<RenderCommandEncoder> s_Encoder;
};

} // namespace Neptune

