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
  Scene(const String& name = "Untitled Scene");
  
  void Reset();
  
  const String& GetName() const { return m_Name; }
  void SetName(const String& name) { m_Name = name; }
  
  template <typename... T>
  auto GetView() { return m_Registry.view<T...>(); }
  template <typename... T>
  auto GetView() const { return m_Registry.view<T...>(); }
  
  template <typename Func>
  auto ForEach(Func func) { m_Registry.each(func); }
  
  Entity CreateEntity(const String& debugName = "Untitled Entity");
  void DestroyEntity(Entity entity);
  
private:
  entt::registry m_Registry;
  String m_Name;
};

} // namespace Neptune
