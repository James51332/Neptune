#pragma once

#include "core/Event.h"
#include "core/Timestep.h"

#include "Camera.h"

namespace Neptune
{

// ----- CameraController -----------------

class CameraController
{
public:
  CameraController(const Camera& camera = Camera());
  
  void OnUpdate(Timestep ts);
  
  Camera& GetCamera() { return m_Camera; }
  const Camera& GetCamera() const { return m_Camera; }
  
private:
  Camera m_Camera;
};

} // namespace Neptune
