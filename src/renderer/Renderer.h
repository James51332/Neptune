#pragma once

#include "core/Event.h"

#include "renderer/RenderContext.h"
#include "renderer/RenderDevice.h"
#include "renderer/Camera.h"
#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "renderer/Buffer.h"
#include "renderer/PipelineState.h"

namespace Neptune
{

class Renderer
{
public:
  static void OnInit(Ref<RenderDevice>& device, Size width, Size height);
  static void OnUpdate();
  static void OnTerminate();
  static void OnEvent(Scope<Event>& event);
  
  static RenderAPI GetAPI() noexcept;
  static Size GetMaxFramesInFlight() noexcept;
  static Size GetFrameNumber() noexcept;
  
  // Eventually we'll move to some form of deferred rendering, so this will become a useful API.
  static void Begin(const Camera& camera);
  static void End();
  
  static void Submit(const Ref<Mesh>& mesh, const Ref<Material>& material);
  static void Submit(const Ref<Model>& model);
  
private:
  static RenderAPI s_API;
  static constexpr Size s_FramesInFlight = 3;
  static Size s_Frame;
  
  static bool s_Recording;
  static Camera s_SceneCamera;
  static Ref<Buffer> s_UniformBuffer;
  static Ref<PipelineState> s_MeshPipeline;
};

} // namespace Neptune
