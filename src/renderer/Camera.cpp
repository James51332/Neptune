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
  // We should translate into camera space, then rotate by pitch, yaw, then roll to affect camera rotation
  // pitch -> y-axis
  // yaw -> x-axis
  // roll - z-axis
  // I can't guarantee this is standard so we could change eventually, but to me it makes sense.
  
  m_View = glm::rotate(glm::mat4(1.0f), glm::radians(-m_Desc.Rotation.z), { 0.0f, 0.0f, 1.0f }) // roll (z-component -> z-axis)
         * glm::rotate(glm::mat4(1.0f), glm::radians(-m_Desc.Rotation.y), { 1.0f, 0.0f, 0.0f }) // yaw (y-component -> x-axis)
  			 * glm::rotate(glm::mat4(1.0f), glm::radians(-m_Desc.Rotation.x), { 0.0f, 1.0f, 0.0f }) // pitch (x-component -> y-axis)
  			 * glm::translate(glm::mat4(1.0f), -m_Desc.Position);

  if (m_Desc.Type == ProjectionType::Perspective)
  {
  	m_Projection = glm::perspective(m_Desc.FOV, m_Desc.Aspect, m_Desc.Near, m_Desc.Far);
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
  
  // Update direction vectors
  m_Forward = glm::normalize(Float3(m_View[2])) * Float3(1 , 1 , -1);
  m_Right = glm::normalize(Float3(m_View[0])) * Float3(1 , 1 , -1);
}

} // namespace Neptune
