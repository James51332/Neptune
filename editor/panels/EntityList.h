#pragma once

#include "Panel.h"

namespace Neptune
{

class EntityList : public Panel
{
public:
  EntityList(Ref<Scene> scene);
  ~EntityList() = default;
  
  virtual void OnImGuiRender() override;
  
private:
  void ShowEntity(Entity e);
  
  void BeginRenameEntity(Entity e);
  void EndRenameEntity();
  
private:
  constexpr static Size s_RenameBufferSize = 64;
  char m_RenameBuffer[s_RenameBufferSize];
  Entity m_RenameEntity;
  bool m_RenameNeedFocus = true;
};

} // namespace Neptune
