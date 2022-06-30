#pragma once

#include "core/Event.h"

#include "renderer/RenderContext.h"
#include "renderer/RenderDevice.h"

namespace Neptune
{

class Renderer
{
public:
  static void OnInit(const Ref<RenderDevice>& device, Size width, Size height);
  static void OnUpdate();
  static void OnTerminate();
  static void OnEvent(Scope<Event>& event);
  
  static RenderAPI GetAPI() noexcept;
  static Size GetMaxFramesInFlight() noexcept;
  static Size GetFrameNumber() noexcept;
  
private:
  static RenderAPI s_API;
  static constexpr Size s_FramesInFlight = 3;
  static Size s_Frame;
};

} // namespace Neptune
