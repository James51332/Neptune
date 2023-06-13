#pragma once

#include "Scene.h"
#include "Entity.h"

namespace Neptune
{

class SceneSerializer
{
public:
  SceneSerializer(Ref<Scene> scene);
  ~SceneSerializer() = default;
  
  void SaveYAML(const String& file);
  void LoadYAML(const String& file);
  
  // TODO: We want to create some API that will allow the scene to persist.
  // For the editor, when we end runtime, we should revert to the scene state
  // prior to running. I'm not sure exactly how we'll implement this, but for
  // now, everything will just be parsed or emitted using YAML. We may want
  // to investigate some form of directly dumping the scene, so the process is
  // more efficient. A goal API may look similar, but there is no emitting.
  // entt does have some form of a snapshot API that we should look into.
  
  // void Save();
  // void Load();
   
  
private:
  Ref<Scene> m_Scene;
};

} // namespace Neptune
