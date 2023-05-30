#include "neptunepch.h"
#include "Scene.h"

#include "Entity.h"

namespace Neptune
{

// ----- Scene -----------------

Scene::Scene()
{
  
}

Entity Scene::CreateEntity()
{
  EntityID ID = m_Registry.create();
  return { ID, this };
}

void Scene::DestroyEntity(Entity entity)
{
  m_Registry.destroy(entity);
}
	
}
