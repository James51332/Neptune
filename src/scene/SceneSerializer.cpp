#include "neptunepch.h"
#include "SceneSerializer.h"

#include "Components.h"

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/emitter.h>
#include <iostream>
#include <fstream>

namespace Neptune
{

// ----- YAML Operators -----------

// Basic Types
YAML::Emitter& operator <<(YAML::Emitter& out, const String& s) {
  out << s.Raw();
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Float2& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Float3& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Float4& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

// Components (each component is a key in the entities map, and each component has its own map of properties)
YAML::Emitter& operator <<(YAML::Emitter& out, const TagComponent& v)
{
  out << YAML::Key << "Tag Component";
  out << YAML::Value << YAML::BeginMap;
  	out << YAML::Key << "Name" << YAML::Value << v.Name;
  out << YAML::EndMap;
  
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const TransformComponent& v)
{
  out << YAML::Key << "Transform Component";
  out << YAML::Value << YAML::BeginMap;
  	out << YAML::Key << "Position" << YAML::Value << v.Position;
  	out << YAML::Key << "Rotation" << YAML::Value << v.Rotation;
  	out << YAML::Key << "Scale" << YAML::Value << v.Scale;
  out << YAML::EndMap;
  
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const SpriteRendererComponent& v)
{
  out << YAML::Key << "Sprite Renderer Component";
  out << YAML::Value << YAML::BeginMap;
  	// out << YAML::Key << "Texture" << YAML::Value << v.Texture; // TODO: Serialize textures
  	out << YAML::Key << "Color" << YAML::Value << v.Color;
  	out << YAML::Key << "Tiling Factor" << YAML::Value << v.TilingFactor;
  out << YAML::EndMap;
  
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const CameraComponent& v)
{
  const CameraDesc& desc = v.Camera.GetDesc();
  
  out << YAML::Key << "Camera Component";
  out << YAML::Value << YAML::BeginMap;
  	if (desc.Type == ProjectionType::Perspective)
  	{
    	out << YAML::Key << "Type" << YAML::Value << "Perspective";
    	out << YAML::Key << "FOV" << YAML::Value << desc.FOV;
  	} else
  	{
    	out << YAML::Key << "Type" << YAML::Value << "Orthographic";
    	out << YAML::Key << "Zoom" << YAML::Value << desc.Zoom;
  	}
  	out << YAML::Key << "Near" << YAML::Value << desc.Near;
  	out << YAML::Key << "Far" << YAML::Value << desc.Far;
  out << YAML::EndMap;
  
  return out;
}

// Entities
YAML::Emitter& operator <<(YAML::Emitter& out, Entity entity)
{
  out << YAML::BeginMap;
  	if (entity.HasComponent<TagComponent>())
      out << entity.GetComponent<TagComponent>();
  	if (entity.HasComponent<TransformComponent>())
      out << entity.GetComponent<TransformComponent>();
  	if (entity.HasComponent<SpriteRendererComponent>())
      out << entity.GetComponent<SpriteRendererComponent>();
  	if (entity.HasComponent<CameraComponent>())
      out << entity.GetComponent<CameraComponent>();
  out << YAML::EndMap;
  
  return out;
}

// ----- SceneSerializer -----------

SceneSerializer::SceneSerializer(Ref<Scene> scene)
	: m_Scene(scene)
{
}

void SceneSerializer::LoadYAML(const String& fileName)
{
  std::ifstream file(fileName.Raw());
  NEPTUNE_ASSERT(file, "Failed to load scene file!");
  
  YAML::Node rootNode = YAML::Load(file);
}

void SceneSerializer::SaveYAML(const String& fileName)
{
  YAML::Emitter out;
  out << YAML::BeginMap;
  
  out << YAML::Key << "Scene" << YAML::Value << fileName;
  out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
  {
   	m_Scene->ForEach([this, &out](entt::entity e)
    {
      Entity entity = { e, m_Scene.Raw() };
      out << entity;
    });
  }
  out << YAML::EndSeq;
  out << YAML::EndMap;
  
  std::ofstream file(fileName.Raw());
  NEPTUNE_ASSERT(file, "Failed to open file when saving scene!");
  file << out.c_str();
  file.close();
}

} // namespace Neptune
