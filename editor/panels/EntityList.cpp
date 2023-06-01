#include "neptunepch.h"
#include "EntityList.h"

#include <imgui/imgui.h>

namespace Neptune
{

EntityList::EntityList(Scene* scene)
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
    auto& tag = view.get<TagComponent>(entity);
  	if (ImGui::TreeNode(tag.Name.Raw()))
    {
      ImGui::TreePop();
    }
  }
  
  ImGui::End();
}

} // namespace Neptune
