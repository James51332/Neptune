#pragma once

#include "Panel.h"

namespace Neptune
{

class EntityList : public Panel
{
public:
  EntityList(Scene* scene);
  ~EntityList() = default;
  
  virtual void OnImGuiRender() override;
};

} // namespace Neptune
