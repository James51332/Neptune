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
  // Transform the camera (pitch, yaw, roll, translate)
  Matrix4 translation = glm::translate(glm::mat4(1.0f), m_Desc.Position);
  Matrix4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Desc.Rotation.x), { 0.0f, 1.0f, 0.0f }) // pitch (x-component -> y-axis)
  * glm::rotate(glm::mat4(1.0f), glm::radians(m_Desc.Rotation.y), { 1.0f, 0.0f, 0.0f }) // yaw (y-component -> x-axis)
  * glm::rotate(glm::mat4(1.0f), glm::radians(m_Desc.Rotation.z), { 0.0f, 0.0f, 1.0f }); // roll (z-component -> z-axis)
  
  Matrix4 transform = translation * rotation;
  m_View = glm::inverse(transform);

  if (m_Desc.Type == ProjectionType::Perspective)
  {
  	m_Projection = glm::perspective(glm::radians(m_Desc.FOV), m_Desc.Aspect, m_Desc.Near, m_Desc.Far);
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
  m_Forward = rotation * Float4(0, 0, -1, 1);
  m_Right = rotation * Float4(1, 0, 0, 1);
}

} // namespace Neptune
