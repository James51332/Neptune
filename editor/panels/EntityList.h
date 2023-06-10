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
};

} // namespace Neptune
