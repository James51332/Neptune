#pragma once

#include "renderer/RenderCommand.h"

#include "MetalCommandBuffer.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLCommandQueue.h>
#import <Metal/MTLCommandBuffer.h>

namespace Neptune
{

// ----- MetalRenderCommandEncoder -----------------

class MetalRenderCommandEncoder final : public RenderCommandEncoder
{
public:
  MetalRenderCommandEncoder(id<MTLDevice> device, const Ref<MetalCommandBufferRegistry>& registry);
  ~MetalRenderCommandEncoder();
  
  void BeginRecording(CommandBuffer& commandBuffer);
  void EndRecording();
  
  void BeginRenderPass(const RenderPass& renderPass);
  void EndRenderPass();
  
  void SetPipelineState(const Ref<PipelineState>& state);
  void SetVertexBuffer(const Ref<Buffer>& buffer, Size index);
  
  void Submit(const DrawCommandDesc& desc);
  
private:
  id<MTLDevice> m_Device;
  Ref<MetalCommandBufferRegistry> m_Registry;
  
  id<MTLCommandBuffer> m_ActiveCommandBuffer;
  id<MTLRenderCommandEncoder> m_ActiveRenderEncoder;
  bool m_Recording = false;
  bool m_RenderPass = false;
};

} // namespace Neptune
