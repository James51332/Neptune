#include "neptunepch.h"
#include "Inspector.h"

#include <imgui/imgui.h>

namespace Neptune
{

Entity Inspector::s_SelectedEntity;

Inspector::Inspector(Ref<Scene> scene)
	: Panel(scene)
{
}

void Inspector::OnImGuiRender()
{
  if (!m_Show) return;
  
  ImGui::Begin("Inspector");
  
  if (s_SelectedEntity)
  {
  	if (s_SelectedEntity.HasComponent<TagComponent>())
    {
      ShowTagEditor();
      ImGui::Separator();
    }
    
    if (s_SelectedEntity.HasComponent<TransformComponent>())
    {
      ShowTransformEditor();
    }
  }
  
  ImGui::End();
}

void Inspector::ShowTagEditor()
{
  auto& tag = s_SelectedEntity.GetComponent<TagComponent>();
  ImGui::Text("Tag Component");
  
  static char buffer[128];
  strcpy(buffer, tag.Name.Raw());
  ImGui::InputText("Name", buffer, 128);
  
  if (strlen(buffer) > 0)
    tag.Name = buffer;
}

void Inspector::ShowTransformEditor()
{
  auto& transform = s_SelectedEntity.GetComponent<TransformComponent>();
  ImGui::Text("Transform Component");
  
  ImGui::DragFloat3("Position", &transform.Position[0], 0.05f);
  ImGui::DragFloat3("Rotation", &transform.Rotation[0], 0.05f);
  ImGui::DragFloat3("Scale", &transform.Scale[0], 0.05f);
}


} // namespace Neptune
