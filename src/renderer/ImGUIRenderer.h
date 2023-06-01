#pragma once

#include "core/Event.h"

#include "renderer/RenderDevice.h"
#include "renderer/Buffer.h"
#include "renderer/PipelineState.h"
#include "renderer/Texture.h"

namespace Neptune
{

// ----- ImGUIRenderer -----------------

struct ImGUIRendererData
{
  Ref<RenderDevice> Device;
  
	// This is an estimate as to how many DrawLists ImGui will max out at.
  constexpr static Int32 NumBuffers = 15;
  Ref<Buffer> VertexBuffers[NumBuffers];
  Ref<Buffer> IndexBuffers[NumBuffers];
  Ref<Buffer> UniformBuffer;
  Size NextBuffer = 0;
  
  Float2 FramebufferSize{800, 600};
  
  Ref<PipelineState> PipelineState;
  
  Ref<Texture> FontTexture;
  
  bool BlockEvents;
};

class ImGUIRenderer
{
  friend class Renderer;
public:
  static void Render();
  static void BlockEvents(bool block = true);
  
private:
  static void OnInit(const Ref<RenderDevice>& device, Size width, Size height);
  static void OnTerminate();
  static void OnEvent(Scope<Event>& event);
private:
  
  static ImGUIRendererData s_Data;
};

} // namespace Neptune
