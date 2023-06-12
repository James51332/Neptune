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
    
    if (s_SelectedEntity.HasComponent<SpriteRendererComponent>())
    {
      ImGui::Separator();
      ShowSpriteEditor();
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

static Int32 ProjectionTypeToInt(ProjectionType type)
{
  switch (type)
  {
    case ProjectionType::Orthographic: return 0;
    case ProjectionType::Perspective: return 1;
    default: NEPTUNE_ASSERT(false, "Unknown Projection Type!"); return -1;
  }
}

static ProjectionType IntToProjectionType(Int32 type)
{
  switch (type)
  {
    case 0: return ProjectionType::Orthographic;
    case 1: return ProjectionType::Perspective;
    default: NEPTUNE_ASSERT(false, "Unknown Projection Type!"); return ProjectionType::Orthographic;
  }
}

void Inspector::ShowCameraEditor()
{
  auto& camera = s_SelectedEntity.GetComponent<CameraComponent>();
  ImGui::Text("Camera Component");
  
  // Get the camera state, and keep track of whether it is updated or not
 	CameraDesc desc = camera.Camera.GetDesc();
  bool changed = false;
  
  // Show button that sets camera as main camera.
  {
  	bool main = (s_SelectedEntity == SceneRenderer::GetRuntimeCamera());
  	if (main) ImGui::BeginDisabled();
  	if (ImGui::Button("Set Primary Runtime Camera")) SceneRenderer::SetRuntimeCamera(s_SelectedEntity);
  	if (main) ImGui::EndDisabled();
  }
  
  // Show the projection type of the camera (using helper function to use ImGui API)
  Int32 mode = ProjectionTypeToInt(desc.Type);
  if (ImGui::Combo("Type", &mode, "Orthographic\0Perspective\0", 2))
  {
    changed = true;
    desc.Type = IntToProjectionType(mode);
  }
  
  // Show the near/far clips
  if (ImGui::DragFloat("Near Clip", &desc.Near)) changed = true;
  if (ImGui::DragFloat("Far Clip", &desc.Far)) changed = true;
  
  // Perspective Cameras have FOV, whereas Orthographic cameras have zoom
  if (desc.Type == ProjectionType::Orthographic)
  {
    if (ImGui::DragFloat("Zoom", &desc.Zoom)) changed = true;
  } else
  {
    if (ImGui::DragFloat("FOV", &desc.FOV)) changed = true;
  }
  
  // Update the camera if its desc is changed
  if (changed)
    camera.Camera.SetDesc(desc);
}

void Inspector::ShowSpriteEditor()
{
  auto& sprite = s_SelectedEntity.GetComponent<SpriteRendererComponent>();
  ImGui::Text("Sprite Renderer Component");
  
  // TODO: API to add textures via GUI
  if (sprite.Texture)
  {
  	ImGui::Image((void*)&sprite.Texture, {64.0f, 64.0f}, {0, 1}, {1, 0});
  	ImGui::SameLine(); ImGui::Text("Texture");
  }
    
  ImGui::ColorEdit4("Color", &sprite.Color[0]);
  ImGui::DragFloat("Tiling Factor", &sprite.TilingFactor);
}


} // namespace Neptune
