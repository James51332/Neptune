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
  
  // Popup (right click on window)
  if (ImGui::BeginPopupContextWindow())
  {
    if (ImGui::MenuItem("Create Entity"))
    {
      Entity entity = m_Scene->CreateEntity();
      BeginRenameEntity(entity);
    }
    
    ImGui::EndPopup();
  }
  
  // Display all entities
  {
//    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0,0});
    
    auto view = m_Scene->GetView<TagComponent>();
  	Int32 id = 1;
  	for (auto entity : view)
  	{
  	  Entity e = {entity, m_Scene.Raw()};
  	
  	  ImGui::PushID(id++);
  	  ShowEntity(e);
  	  ImGui::PopID();
  	}
    
//    ImGui::PopStyleVar();
  }
  
  ImGui::End();
}

// Shows UI to add component if the entity has the type
template <typename T>
static void ShowAddComponent(Entity entity, const char* text)
{
  // Don't show if entity has component
  bool has = entity.HasComponent<T>();
  if (has) ImGui::BeginDisabled();
  
  // TODO: We may need to use factories or other systems in the future to ensure dependent components.
  if (ImGui::MenuItem(text)) entity.AddComponent<T>();
  
  if (has) ImGui::EndDisabled();
}

void EntityList::ShowEntity(Entity entity)
{
  bool rename = (entity == m_RenameEntity);
  bool selected = (entity == Inspector::GetSelectedEntity());
  auto& tag = entity.GetComponent<TagComponent>();
  
  // Show a text field for renaming entities
  if (!rename)
  {
    // List the entity as a tree node
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf
    												 | ImGuiTreeNodeFlags_FramePadding
                             | ImGuiTreeNodeFlags_SpanFullWidth
    												 | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    
    if (selected) flags |= ImGuiTreeNodeFlags_Selected;
    
    ImGui::AlignTextToFramePadding();
    ImGui::TreeNodeEx(tag.Name.Raw(), flags);
    
    // Select if entity is clicked
    if (ImGui::IsItemClicked())
    	Inspector::SetSelectedEntity(entity);
    
    // Rename entity
    if (selected && Input::KeyPress(KeyEnter) && ImGui::IsWindowFocused())
      BeginRenameEntity(entity);
    
    // Right click on entity
    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Rename Entity")) BeginRenameEntity(entity);
      
      if (ImGui::BeginMenu("Add Component"))
      {
        ShowAddComponent<SpriteRendererComponent>(entity, "Sprite Renderer Component");
        ShowAddComponent<CameraComponent>(entity, "Camera Component");
        
        ImGui::EndMenu();
      }
      
      if (ImGui::MenuItem("Delete Entity")) m_Scene->DestroyEntity(entity);
      
      ImGui::EndPopup();
    }
  } else
  {
    if (m_RenameNeedFocus)
    {
      ImGui::SetKeyboardFocusHere();
      m_RenameNeedFocus = false;
    }
    
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::InputText("##entityname", m_RenameBuffer, s_RenameBufferSize, ImGuiInputTextFlags_EnterReturnsTrue);
    
    // If we're done editting, rename the entity. ImGui is weird and deactivate immediately
    // if enter is down. We only want to end if deactivated by focus loss. We'll manually end if enter is pressed.
    if ((ImGui::IsItemDeactivated() && !Input::KeyDown(KeyEnter)) || Input::KeyPress(KeyEnter))
      EndRenameEntity();
  }
}

void EntityList::BeginRenameEntity(Entity entity)
{
  // Set the rename buffer equal to name
  std::strcpy(m_RenameBuffer, entity.GetComponent<TagComponent>().Name.Raw());
  
  m_RenameEntity = entity;
  Inspector::SetSelectedEntity(m_RenameEntity);
  
  m_RenameNeedFocus = true;
}

void EntityList::EndRenameEntity()
{
  if (std::strlen(m_RenameBuffer) > 0) // Only rename if the string is not empty
    m_RenameEntity.GetComponent<TagComponent>().Name = m_RenameBuffer;
  
  m_RenameEntity = { entt::null, nullptr };
}

} // namespace Neptune
