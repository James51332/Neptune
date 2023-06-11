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
  
private:
  bool m_OpenRename = false; // Set true the first time the a rename is requested (set text buffer, etc.)
  Entity m_RenameEntity; // Stores the entity that is renamed
};

} // namespace Neptune
