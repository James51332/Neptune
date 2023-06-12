#pragma once

#include "Panel.h"

namespace Neptune
{

class Inspector : public Panel
{
public:
  Inspector(Ref<Scene> scene);
  ~Inspector() = default;
  
  virtual void OnImGuiRender() override;
  
  static void SetSelectedEntity(Entity e) { s_SelectedEntity = e; }
  static Entity GetSelectedEntity() { return s_SelectedEntity; }
  
private:
  void ShowTagEditor();
  void ShowTransformEditor();
  void ShowCameraEditor();
  void ShowSpriteEditor();
  
private:
  static Entity s_SelectedEntity;
};

} // namespace Neptune
