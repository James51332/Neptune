#include "neptunepch.h"
#include "SceneSerializer.h"

#include "Components.h"
#include "SceneRenderer.h"

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/emitter.h>
#include <iostream>
#include <fstream>

// ----- YAML Emitter Operators -----------

// Basic Types
namespace Neptune
{

YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::String& s) {
  out << s.Raw();
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::Float2& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::Float3& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::Float4& v)
{
  out << YAML::Flow;
  out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
  return out;
}

// Components (each component is a key in the entities map, and each component has its own map of properties)
YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::TagComponent& v)
{
  out << YAML::Key << "Tag Component";
  out << YAML::Value << YAML::BeginMap;
  	out << YAML::Key << "Name" << YAML::Value << v.Name;
  out << YAML::EndMap;
  
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::TransformComponent& v)
{
  out << YAML::Key << "Transform Component";
  out << YAML::Value << YAML::BeginMap;
  	out << YAML::Key << "Position" << YAML::Value << v.Position;
  	out << YAML::Key << "Rotation" << YAML::Value << v.Rotation;
  	out << YAML::Key << "Scale" << YAML::Value << v.Scale;
  out << YAML::EndMap;
  
  return out;
}

YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::SpriteRendererComponent& v)
{
  out << YAML::Key << "Sprite Renderer Component";
  out << YAML::Value << YAML::BeginMap;
  	// out << YAML::Key << "Texture" << YAML::Value << v.Texture; // TODO: Serialize textures
  	out << YAML::Key << "Color" << YAML::Value << v.Color;
  	out << YAML::Key << "Tiling Factor" << YAML::Value << v.TilingFactor;
  out << YAML::EndMap;
  
  return out;
}

// HACK: This is a nasty way of communicating state for the operator.
static bool emitRuntime = false;
YAML::Emitter& operator <<(YAML::Emitter& out, const Neptune::CameraComponent& v)
{
  const Neptune::CameraDesc& desc = v.Camera.GetDesc();
  
  out << YAML::Key << "Camera Component";
  out << YAML::Value << YAML::BeginMap;
  	out << YAML::Key << "Runtime" << YAML::Value << (emitRuntime);
    if (desc.Type == Neptune::ProjectionType::Perspective)
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
YAML::Emitter& operator <<(YAML::Emitter& out, Neptune::Entity entity)
{
  out << YAML::BeginMap;
  	if (entity.HasComponent<Neptune::TagComponent>())
      out << entity.GetComponent<Neptune::TagComponent>();
  	if (entity.HasComponent<Neptune::TransformComponent>())
      out << entity.GetComponent<Neptune::TransformComponent>();
  	if (entity.HasComponent<Neptune::SpriteRendererComponent>())
      out << entity.GetComponent<Neptune::SpriteRendererComponent>();
  	if (entity.HasComponent<Neptune::CameraComponent>())
    {
      emitRuntime = entity == SceneRenderer::GetRuntimeCamera();
      out << entity.GetComponent<Neptune::CameraComponent>();
    }
  out << YAML::EndMap;
  
  return out;
}

} // namespace Neptune

// ----- YAML Parser Operators -----------

namespace YAML
{

template<>
struct convert<Neptune::String>
{
  static bool decode(const Node& node, Neptune::String& rhs) {
    if(!node.IsScalar()) {
      return false;
    }
    
    rhs = node.Scalar().c_str();
    return true;
  }
};

template<>
struct convert<Neptune::Float2>
{
  static bool decode(const Node& node, Neptune::Float2& rhs) {
    if(!node.IsSequence() || node.size() != 2) {
      return false;
    }
    
    rhs.x = node[0].as<Float32>();
    rhs.y = node[1].as<Float32>();
    return true;
  }
};

template<>
struct convert<Neptune::Float3>
{
  static bool decode(const Node& node, Neptune::Float3& rhs) {
    if(!node.IsSequence() || node.size() != 3) {
      return false;
    }
    
    rhs.x = node[0].as<Float32>();
    rhs.y = node[1].as<Float32>();
    rhs.z = node[2].as<Float32>();
    return true;
  }
};

template<>
struct convert<Neptune::Float4>
{
  static bool decode(const Node& node, Neptune::Float4& rhs) {
    if(!node.IsSequence() || node.size() != 4) {
      return false;
    }
    
    rhs.x = node[0].as<Float32>();
    rhs.y = node[1].as<Float32>();
    rhs.z = node[2].as<Float32>();
    rhs.w = node[3].as<Float32>();
    return true;
  }
};

} // namespace YAML

namespace Neptune
{

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
  
  m_Scene->Reset();
  
  m_Scene->SetName(rootNode["Scene"].as<String>());
  YAML::Node entities = rootNode["Entities"];
  for (Size i = 0; i < entities.size(); ++i)
  {
    YAML::Node entity = entities[i];
    
    // Tag
    Entity e = m_Scene->CreateEntity(entity["Tag Component"]["Name"].as<String>());
    
    // Transform
    {
    	auto& transform = e.GetComponent<TransformComponent>();
    	transform.Position = entity["Transform Component"]["Position"].as<Float3>();
    	transform.Rotation = entity["Transform Component"]["Rotation"].as<Float3>();
    	transform.Scale = entity["Transform Component"]["Scale"].as<Float3>();
    }
      
    // Sprite
    if (entity["Sprite Renderer Component"])
    {
      auto& sprite = e.AddComponent<SpriteRendererComponent>();
      sprite.Color = entity["Sprite Renderer Component"]["Color"].as<Float4>();
      sprite.TilingFactor = entity["Sprite Renderer Component"]["Tiling Factor"].as<Float32>();
    }
    
    // Camera
    if (entity["Camera Component"])
    {
      CameraDesc desc;
      {
      	if (entity["Camera Component"]["Type"].as<std::string>() == "Perspective")
      	{
          desc.Type = ProjectionType::Perspective;
          desc.FOV = entity["Camera Component"]["FOV"].as<Float32>();
      	} else
      	{
          desc.Type = ProjectionType::Orthographic;
          desc.Zoom = entity["Camera Component"]["Zoom"].as<Float32>();
      	}
        
        desc.Near = entity["Camera Component"]["Near"].as<Float32>();
        desc.Far = entity["Camera Component"]["Far"].as<Float32>();
      }
      
     	e.AddComponent<CameraComponent>(desc);
      
      if (entity["Camera Component"]["Runtime"].as<bool>(false))
      	SceneRenderer::SetRuntimeCamera(e);
    }
  }
}

void SceneSerializer::SaveYAML(const String& fileName)
{
  YAML::Emitter out;
  out << YAML::BeginMap;
  
  out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();
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
