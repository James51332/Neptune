#pragma once

#include "renderer/Texture.h"

namespace Neptune
{

// ----- Material -----------------

// Materials represents uniforms and data. For now, all
// materials will be PBR, because I can't think of any other
// use case. We should probably support

struct MaterialDesc
{
  Float4 Ambient;
  Float4 Diffuse;
  Float4 Specular;
  Float32 Shininess;
};

class Material
{
  friend class MetalRenderDevice;
  
public:
  ~Material() = default;
  
private:
  Material(const MaterialDesc& desc);
  
  Float4 m_Ambient;
  Float4 m_Diffuse;
  Float4 m_Specular;
  Float32 m_Shininess;
};

} // namespace Neptune

