#include "neptunepch.h"
#include "SceneManager.h"

#include "Components.h"

namespace Neptune
{

Scene* SceneManager::s_Scene = nullptr;
bool SceneManager::s_Runtime = false;

void SceneManager::OnInit(Scene* scene, bool runtime)
{
  NEPTUNE_ASSERT(!s_Scene && scene, "Cannot initialize SceneManager");
  s_Scene = scene;
  
  SetRuntime(runtime);
}

void SceneManager::OnTerminate()
{
  SetRuntime(false);
}

void SceneManager::ChangeScene(Scene* scene)
{
  if (s_Runtime) TerminateScripts();

  s_Scene = scene;
  
  if (s_Runtime) InitScripts();
}

void SceneManager::OnUpdate()
{
  if (s_Runtime)
    UpdateScripts();
}

void SceneManager::SetRuntime(bool runtime)
{
  if (runtime && !s_Runtime) InitScripts();
  if (!runtime && s_Runtime) TerminateScripts();
  
  s_Runtime = runtime;
}

void SceneManager::InitScripts()
{
  auto view = s_Scene->GetView<NativeScriptComponent>();
  for (auto entt : view)
  {
    auto& script = view.get<NativeScriptComponent>(entt);
    script.Script->OnInit({entt, s_Scene});
  }
}

void SceneManager::UpdateScripts()
{
  auto view = s_Scene->GetView<NativeScriptComponent>();
  for (auto entt : view)
  {
    auto& script = view.get<NativeScriptComponent>(entt);
    script.Script->OnUpdate({entt, s_Scene});
  }
}

void SceneManager::TerminateScripts()
{
  auto view = s_Scene->GetView<NativeScriptComponent>();
  for (auto entt : view)
  {
    auto& script = view.get<NativeScriptComponent>(entt);
    script.Script->OnTerminate({entt, s_Scene});
  }
}

} // namespace Neptune
