#pragma once

#include "panels/Panel.h"
#include "panels/Viewport.h"

#include <Neptune/Neptune.h>

// ----- EditorLayer -----------------

namespace Neptune
{

class EditorLayer : public Layer
{
public:
  EditorLayer();
  ~EditorLayer();
  
  virtual void OnInit(const Ref<RenderDevice>& device) override;
  virtual void OnTerminate() override;
  virtual void OnUpdate() override;
  virtual void OnRender(const Ref<Framebuffer>& framebuffer) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Scope<Event>& e) override;
  
private:
  Ref<RenderDevice> m_RenderDevice;
  
  DynamicArray<Panel*> m_Panels;
  Viewport* m_Viewport;
  
  Scene m_Scene;
  Entity m_Entity;
  Entity m_CameraEntity;
};

} // namespace Neptune
