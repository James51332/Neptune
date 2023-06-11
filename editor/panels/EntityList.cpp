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
      Entity e = m_Scene->CreateEntity();
      m_RenameEntity = e;
      m_OpenRename = true;
    }
    
    ImGui::EndPopup();
  }
  
  // Display all entities
  auto view = m_Scene->GetView<TagComponent>();
  for (auto entity : view)
  {
    Entity e = {entity, m_Scene.Raw()};
    ShowEntity(e);
  }
  
  ImGui::End();
}

void EntityList::ShowEntity(Entity e)
{
  bool rename = (e == m_RenameEntity);
  bool selected = (e == Inspector::GetSelectedEntity());
  auto& tag = e.GetComponent<TagComponent>();
  
  // Show a text field for renaming entities
  if (!rename)
  {
    // List the entity as a selectable
  	if (ImGui::Selectable(tag.Name.Raw(), selected))
  	{
  	  Inspector::SetSelectedEntity(e);
  	}
    
    // Right click on entity
    if (ImGui::BeginPopupContextItem())
    {
      if (ImGui::MenuItem("Rename Entity"))
      {
        m_OpenRename = true;
        m_RenameEntity = e;
      }
      
      if (ImGui::MenuItem("Delete Entity"))
      {
        m_Scene->DestroyEntity(e);
      }
      
      ImGui::EndPopup();
    }
  } else
  {
    // If this is first frame editting entity, set input text and focus it.
    constexpr Size bufferSize = 64;
    static char buffer[bufferSize];
    if (m_OpenRename)
    {
      std::strcpy(buffer, tag.Name.Raw());
      ImGui::SetKeyboardFocusHere();
    }
    
    // Draw the text input field
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x); // Span the entire width of the list
    ImGui::InputText("##entityname", buffer, bufferSize);
    bool active = ImGui::IsItemActive();
    
    // If we're done editting, rename the entity.
    if (Input::KeyPress(KeyEnter) || (!active && !m_OpenRename))
    {
      if (std::strlen(buffer) > 0) // Only rename if the string is not empty
      	tag.Name = buffer;
      m_RenameEntity = { entt::null, nullptr };
    }
    
    // If we're already renaming, we don't need this (only true for first frame)
    m_OpenRename = false;
  }
}

} // namespace Neptune
