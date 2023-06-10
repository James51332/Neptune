#include "neptunepch.h"
#include "CameraController.h"

#include "core/Input.h"

namespace Neptune
{

// ----- CameraController -----------------

CameraController::CameraController(const Camera& cam)
	: m_Camera(cam)
{
}

void CameraController::OnUpdate(Timestep ts)
{
  CameraDesc desc = m_Camera.GetDesc();
    
  if (Input::KeyDown(KeyW)) desc.Position += m_Camera.GetForwardDirection() * 7.0f * (Float32)ts;
  if (Input::KeyDown(KeyS)) desc.Position += m_Camera.GetForwardDirection() * -7.00f * (Float32)ts;
  if (Input::KeyDown(KeyA)) desc.Position += m_Camera.GetRightDirection() * -7.0f * (Float32)ts;
  if (Input::KeyDown(KeyD)) desc.Position += m_Camera.GetRightDirection() * 7.0f * (Float32)ts;
  if (Input::KeyDown(KeySpace)) desc.Position.y += 0.05f;
  if (Input::KeyDown(KeyLeftShift)) desc.Position.y += -0.05f;
  if (Input::MouseDown(MouseLeft)) desc.Rotation.x += Input::GetMouseDeltaX() * 0.1f;
  if (Input::MouseDown(MouseLeft))
  {
    // Max yaw at 90 degrees
    desc.Rotation.y += Input::GetMouseDeltaY() * 0.1f;
    if (glm::abs(desc.Rotation.y) > 90.0f)
      desc.Rotation.y = 90.0f * glm::sign(desc.Rotation.y);
  }
    
  m_Camera.SetDesc(desc);
}

} // namespace Neptune
