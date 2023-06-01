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
  
  template <typename... T>
  auto GetView() { return m_Registry.view<T...>(); }
  template <typename... T>
  auto GetView() const { return m_Registry.view<T...>(); }
  
  Entity CreateEntity();
  void DestroyEntity(Entity entity);
  
private:
  entt::registry m_Registry;
};

} // namespace Neptune
