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
    }
    
    if (s_SelectedEntity.HasComponent<TransformComponent>())
    {
      ImGui::Separator();
      ShowTransformEditor();
    }
    
    if (s_SelectedEntity.HasComponent<CameraComponent>())
    {
      ImGui::Separator();
      ShowCameraEditor();
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

void Inspector::ShowCameraEditor()
{
  //auto& camera = s_SelectedEntity.GetComponent<CameraComponent>();
  ImGui::Text("Camera Component");
  
 // CameraDesc desc = camera.Camera.GetDesc();
  
//  if (ImGui::Combo(label, &style_idx, "Orthographic\0Perspective\0"))
//  {
//    switch (type)
//    {
//      case 0: ImGui::StyleColorsDark(); break;
//      case 1: ImGui::StyleColorsLight(); break;
//      case 2: ImGui::StyleColorsClassic(); break;
//    }
//  }
}


} // namespace Neptune
