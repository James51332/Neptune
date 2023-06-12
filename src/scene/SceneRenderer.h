#pragma once

#include "Scene.h"

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
  
  // Use appropriate camera (based on if application is in runtime)
  static void RenderEditor(const Camera& camera);
  
  // Use the runtime camera (for sandbox, which doesn't provide an editor camera)
  static void RenderRuntime();

private:
  static void Render(const Camera& camera);
  
private:
  static Ref<Scene> s_Scene;
  static Float32 s_AspectRatio;
};

} // namespace Neptune
