#include "neptunepch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

namespace Neptune
{

// ----- Scene -----------------

Scene::Scene(const String& name)
	: m_Name(name)
{
}

void Scene::Reset()
{
  m_Registry.clear();
}

Entity Scene::CreateEntity(const String& debugName)
{
  EntityID ID = m_Registry.create();
  
  // For now, there's no reason that all of our entities don't have a tag or transform
  Entity e{ ID, this };
  e.AddComponent<TagComponent>(debugName);
  e.AddComponent<TransformComponent>();
  
  return e;
}

void Scene::DestroyEntity(Entity entity)
{
  m_Registry.destroy(entity);
}
	
}
