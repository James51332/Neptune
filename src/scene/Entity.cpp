#include "neptunepch.h"
#include "Entity.h"

namespace Neptune
{

// ----- Entity ------------

Entity::Entity(EntityID ID, Scene* scene)
	: m_ID(ID), m_Scene(scene) {}

} // namespace Neptune
