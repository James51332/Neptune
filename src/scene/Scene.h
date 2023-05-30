#pragma once

#include <entt/entt.hpp>

namespace Neptune
{

class Entity;

// ----- Scene -----------------

class Scene
{
  friend class Entity;
public:
  Scene();
  
  Entity CreateEntity();
  void DestroyEntity(Entity entity);
  
private:
  entt::registry m_Registry;
};

} // namespace Neptune
