#pragma once

#include "Scene.h"

#include <entt/entt.hpp>

namespace Neptune
{

// ----- Entity ------------

using EntityID = entt::entity;

struct Entity
{
	Entity()
  	: m_ID(), m_Scene(nullptr) {}
  
  template <typename T, typename... Args>
  T& AddComponent(Args&& ...args)
  {
    return m_Scene->m_Registry.emplace<T>(m_ID, std::forward<Args>(args)...);
  }
  
  template <typename T>
  void RemoveComponent()
  {
    m_Scene->m_Registry.remove<T>(m_ID);
  }
  
  template <typename T>
  auto GetComponent()
  {
    return m_Scene->m_Registry.get<T>(m_ID);
  }
  
  inline const EntityID GetEntityID() const { return m_ID; }
  operator EntityID() const { return m_ID; }
  
  Entity(EntityID ID, Scene* scene);
private:
  EntityID m_ID;
  Scene* m_Scene; // We'll assume that these won't outlive the scene.
};

} // namespace Neptune
