#include "neptunepch.h"
#include "Material.h"

namespace Neptune
{

// ----- Material -----------------

Material::Material(const MaterialDesc& desc)
	: m_Ambient(desc.Ambient), m_Diffuse(desc.Diffuse), m_Specular(desc.Specular), m_Shininess(desc.Shininess)
{
}

} // namespace Neptune
