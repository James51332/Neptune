#pragma once

#include "Entity.h"

#include "renderer/Camera.h"
#include "renderer/Texture.h"

#include "core/Timestep.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{

// ----- Tag ------------

struct TagComponent
{
  String Name;
};

// ----- Transform ------------

struct TransformComponent
{
  Float3 Position;
  Float3 Rotation;
  Float3 Scale;
  
  Matrix4 Matrix = Matrix4(1.0f);
  
  // We break the ECS paradigm, but this will save resources to not have to recalculate each frame
  void CalculateTransformMatrix()
  {
    Matrix = glm::translate(glm::mat4(1.0f), Position)
    * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), { 0.0f, 1.0f, 0.0f })
    * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), { 1.0f, 0.0f, 0.0f })
    * glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), { 0.0f, 0.0f, 1.0f })
    * glm::scale(glm::mat4(1.0f), Scale);
  }
  
  TransformComponent()
  	: Position(0.0f), Rotation(0.0f), Scale(1.0f) {}
};

// ----- SpriteRenderer ------------

struct SpriteRendererComponent
{
  Ref<Texture> Texture;
  Float4 Color = Float4(1.0f);
  Float32 TilingFactor = 1.0f;
};

// ----- CameraComponent ------------

struct CameraComponent
{
  Camera Camera;
  bool MainCamera = false;
};

// ----- NativeScriptComponent -------------

// All native scripts must be children of this class
class NativeScript
{
public:
  virtual ~NativeScript() = default;
  
  virtual void OnInit(Entity e) {}
  virtual void OnUpdate(Entity e, Timestep ts) {}
  virtual void OnTerminate(Entity e) {}
};

struct NativeScriptComponent
{
  NativeScript* Script;
};

}
