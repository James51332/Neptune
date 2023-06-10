#pragma once

#include "Scene.h"

#include "core/Timestep.h"

namespace Neptune
{

// Responsible for all game logic of scene system.
class SceneManager
{
public:
  static void OnInit(Ref<Scene> scene, bool runtime = false);
  static void OnTerminate();
  
  static void ChangeScene(Ref<Scene> scene);
  
  static void OnUpdate(Timestep ts);
  
  static void SetRuntime(bool runtime = true);
  static bool GetRuntime() { return s_Runtime; }
  static void ToggleRuntime() { SetRuntime(!s_Runtime); }
  
private:
  static void InitScripts();
  static void UpdateScripts(Timestep ts);
  static void TerminateScripts();
  
private:
  static Ref<Scene> s_Scene;
  static bool s_Runtime;
};

} // namespace Neptune
