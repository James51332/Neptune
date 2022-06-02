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

void RenderCommand::DrawTriangles(Size start, Size count)
{
  s_Encoder->DrawTriangles(start, count);
}

} // namespace Neptune

