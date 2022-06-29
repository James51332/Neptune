#include "neptunepch.h"
#include "Camera.h"

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{

// ----- Camera -----------------

Camera::Camera(const CameraDesc& desc)
{
  SetDesc(desc);
}

void Camera::UpdateMatrices()
{
  m_View = glm::translate(glm::mat4(1.0f), -m_Desc.Position)
      	 * glm::rotate(glm::mat4(1.0f), m_Desc.Rotation.x, { 1.0f, 0.0f, 0.0f})
         * glm::rotate(glm::mat4(1.0f), m_Desc.Rotation.y, { 0.0f, 1.0f, 0.0f})
         * glm::rotate(glm::mat4(1.0f), m_Desc.Rotation.z, { 0.0f, 0.0f, 1.0f});

  if (m_Desc.Type == ProjectionType::Perspective)
  {
  	m_Projection = glm::perspective(m_Desc.FOV, m_Desc.Aspect, 0.1f, 10.0f);
  } else if (m_Desc.Type == ProjectionType::Orthographic)
  {
    Float32 height = (2 / m_Desc.Zoom);
    Float32 width = m_Desc.Aspect * height;
    m_Projection = glm::ortho(m_Desc.Position.x - (width / 2),
                              m_Desc.Position.x + (width / 2),
                              m_Desc.Position.y - (height / 2),
                              m_Desc.Position.y + (height / 2),
                              m_Desc.Near,
                              m_Desc.Far);
  }
  
  m_ViewProjection = m_Projection * m_View;
}

} // namespace Neptune
