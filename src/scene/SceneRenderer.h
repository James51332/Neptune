#pragma once

#include "Scene.h"
#include "Entity.h"

#include "renderer/Camera.h"

namespace Neptune
{

class SceneRenderer
{
public:
  static void OnInit(Ref<Scene> scene);
  static void OnTerminate();
  static void OnResize(Float32 aspect);
  
  static void ChangeScene(Ref<Scene> scene);
  
  // Render the scene using a given camera.
  static void Render(const Camera& camera);
  
  // Use the runtime camera
  static void RenderRuntime();

  // API for managing which camera is used in runtime.
  static void SetRuntimeCamera(Entity camera) { s_RuntimeCamera = camera; }
  static Entity GetRuntimeCamera() { return s_RuntimeCamera; }
  
private:
  static Ref<Scene> s_Scene;
  static Float32 s_AspectRatio;
  
  static Entity s_RuntimeCamera;
};

} // namespace Neptune
