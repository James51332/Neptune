#include "neptunepch.h"
#include "EntityList.h"

#include "Inspector.h"

#include <imgui/imgui.h>

namespace Neptune
{

EntityList::EntityList(Ref<Scene> scene)
	: Panel(scene)
{
}

void EntityList::OnImGuiRender()
{
  if (!m_Show) return;
  
  ImGui::Begin("Entities");
  
  auto view = m_Scene->GetView<TagComponent>();
  for (auto entity : view)
  {
    Entity e = {entity, m_Scene.Raw()};
    bool selected = (e == Inspector::GetSelectedEntity());
    
    auto& tag = view.get<TagComponent>(entity);
    if (ImGui::Selectable(tag.Name.Raw(), selected))
    {
      Inspector::SetSelectedEntity(e);
    }
  }
  
  ImGui::End();
}

} // namespace Neptune
