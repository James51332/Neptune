#pragma once

#include "core/Event.h"

#include "Camera.h"

namespace Neptune
{

// ----- CameraController -----------------

class CameraController
{
public:
  CameraController(const Camera& camera = Camera());
  
  // TODO: Timesteps
  void OnUpdate();
  
  Camera& GetCamera() { return m_Camera; }
  const Camera& GetCamera() const { return m_Camera; }
  
private:
  Camera m_Camera;
};

} // namespace Neptune
