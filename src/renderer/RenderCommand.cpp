#include "neptunepch.h"
#include "RenderCommand.h"

#include "RenderDevice.h"

namespace Neptune
{

// ----- RenderCommand -----------------

Ref<RenderCommandEncoder> RenderCommand::s_Encoder;

void RenderCommand::OnInit(Ref<RenderDevice> device)
{
  s_Encoder = device->GetEncoder();
}

void RenderCommand::OnTerminate()
{
}

void RenderCommand::BeginRecording(CommandBuffer& commandBuffer)
{
  s_Encoder->BeginRecording(commandBuffer);
}

void RenderCommand::EndRecording()
{
  s_Encoder->EndRecording();
}

void RenderCommand::BeginRenderPass(const RenderPass& renderPass)
{
  s_Encoder->BeginRenderPass(renderPass);
}

void RenderCommand::EndRenderPass()
{
  s_Encoder->EndRenderPass();
}

void RenderCommand::SetPipelineState(const Ref<PipelineState>& state)
{
  s_Encoder->SetPipelineState(state);
}

void RenderCommand::SetVertexBuffer(const Ref<Buffer>& buffer, Size index)
{
  s_Encoder->SetVertexBuffer(buffer, index);
}

void RenderCommand::SetClipRect(Size x, Size y, Size w, Size h)
{
  s_Encoder->SetClipRect(x, y, w, h);
}

void RenderCommand::BindTexture(const Ref<Texture>& texture, Size index)
{
  s_Encoder->BindTexture(texture, index);
}

void RenderCommand::Submit(const DrawCommandDesc& desc)
{
  s_Encoder->Submit(desc);
}

} // namespace Neptune

