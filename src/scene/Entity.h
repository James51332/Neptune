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
  	: m_ID(entt::null), m_Scene(nullptr) {}
  
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
  decltype(auto) GetComponent()
  {
    return m_Scene->m_Registry.get<T>(m_ID);
  }
  
  // TODO: The try_get api probably could save some time fetching a second time.
  // We could expose to user if there are issues.
  template <typename T>
  bool HasComponent()
  {
    return m_Scene->m_Registry.try_get<T>(m_ID) != nullptr;
  }
  
  inline const EntityID GetEntityID() const { return m_ID; }
  operator EntityID() const { return m_ID; }
  operator bool() const { if (!m_Scene) return false; return m_Scene->m_Registry.valid(m_ID); }
  bool operator==(const Entity other) const { return m_ID == other.m_ID && m_Scene == other.m_Scene; }
  
  Entity(EntityID ID, Scene* scene);
private:
  EntityID m_ID;
  Scene* m_Scene; // We'll assume that these won't outlive the scene.
};

} // namespace Neptune
