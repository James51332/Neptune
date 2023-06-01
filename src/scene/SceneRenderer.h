#pragma once

#include "Scene.h"

#include "renderer/Camera.h"

namespace Neptune
{

class SceneRenderer
{
public:
  static void OnInit(Scene* scene);
  static void OnTerminate();
  
  static void ChangeScene(Scene* scene);
  
  // Use an editor camera
  static void Render(const Camera& camera);
  
  // Finds and use the game camera
  static void RenderRuntime();
  
private:
  static Scene* s_Scene;
};

} // namespace Neptune
