#pragma once

namespace Neptune
{

// ----- Camera -----------------

enum class ProjectionType
{
  Orthographic,
  Perspective
};

struct CameraDesc
{
  ProjectionType Type = ProjectionType::Perspective;
  
  // For Both
  Float3 Position = { 0.0f, 0.0f, -5.0f };
  Float3 Rotation = { 0.0f, 0.0f, 0.0f };
  Float32 Near = 0.1f;
  Float32 Far = 100.0f;
  Float32 Aspect = 1.0f;
  Float32 Zoom = 1.0f;
  
  // For Perspective
  Float32 FOV = 65.0f;
};

class Camera
{
public:
  Camera() = default;
  Camera(const CameraDesc& desc);
  ~Camera() noexcept = default;
  
  const CameraDesc& GetDesc() const noexcept { return m_Desc; }
  void SetDesc(const CameraDesc& desc) noexcept { m_Desc = desc; UpdateMatrices(); }
  
  const Matrix4& GetViewMatrix() const noexcept { return m_View; }
  const Matrix4& GetProjectionMatrix() const noexcept { return m_Projection; }
  const Matrix4& GetViewProjectionMatrix() const noexcept { return m_ViewProjection; }
  
private:
  void UpdateMatrices();
    
private:
  CameraDesc m_Desc;
  
  Matrix4 m_View = Matrix4(1.0f);
  Matrix4 m_Projection = Matrix4(1.0f);
  Matrix4 m_ViewProjection = Matrix4(1.0f);
};

} // namespace Neptune
