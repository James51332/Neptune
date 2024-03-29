#pragma once

#include <Neptune/Neptune.h>

namespace Neptune
{

// Interface for Editor to use for all panels (not mandated)
class Panel
{
public:
  Panel(Ref<Scene> scene)
  	: m_Scene(scene) {}
  virtual ~Panel() = default;
  
  virtual void OnUpdate(Timestep ts) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Scope<Event>& e) {}
  
  virtual void Show() { m_Show = true; }
  virtual void Hide() { m_Show = false; }

protected:
  Ref<Scene> m_Scene; // It is assumed that panels will be destroyed before the editor
  bool m_Show = true;
};


} // namespace Neptune
