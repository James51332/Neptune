#pragma once

#include "core/Event.h"

#include "renderer/RenderDevice.h"
#include "renderer/Buffer.h"
#include "renderer/PipelineState.h"
#include "renderer/Texture.h"
#include "renderer/Swapchain.h"

namespace Neptune
{

// ----- ImGUIRenderer -----------------

struct ImGUIRendererData
{
  Ref<RenderDevice> Device;
  
  // We want to have multiple buffers as backups since they are stored until after
  // the frame for the command buffer.
  constexpr static Int32 NumBuffers = 15;
  Ref<Buffer> VertexBuffers[NumBuffers];
  Ref<Buffer> IndexBuffers[NumBuffers];
  Ref<Buffer> UniformBuffer;
  Size NextBuffer = 0;
  
  Ref<PipelineState> PipelineState;
  
  Ref<Texture> FontTexture;
};

class ImGUIRenderer
{
public:
  static void OnInit(const Ref<RenderDevice>& device, Size width, Size height);
  static void OnUpdate();
  static void OnTerminate();
  static void OnEvent(Scope<Event>& event);
  
  static void Render(const Ref<Framebuffer>& fb);
  
private:
  static ImGUIRendererData s_Data;
};

} // namespace Neptune
