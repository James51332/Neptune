#pragma once

#include "Scene.h"

namespace Neptune
{

// Responsible for all game logic of scene system.
class SceneManager
{
public:
  static void OnInit(Scene* scene, bool runtime = false);
  static void OnTerminate();
  
  static void ChangeScene(Scene* scene);
  
  static void OnUpdate();
  
  static void SetRuntime(bool runtime = true);
  static bool GetRuntime() { return s_Runtime; }
  static void ToggleRuntime() { SetRuntime(!s_Runtime); }
  
private:
  static void InitScripts();
  static void UpdateScripts();
  static void TerminateScripts();
  
private:
  static Scene* s_Scene;
  static bool s_Runtime;
};

} // namespace Neptune
