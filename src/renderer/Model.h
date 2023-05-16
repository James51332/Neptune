#pragma once

#include "renderer/Mesh.h"
#include "renderer/Material.h"

namespace Neptune
{

class RenderDevice;

// ----- Model -----------------

struct ModelDesc
{
  String Path;
};

class Model
{
  friend class MetalRenderDevice;
  
  friend class Renderer;
public:
  ~Model() = default;
  
private:
  Model(RenderDevice* device, const ModelDesc& desc);
  
  DynamicArray<Ref<Mesh>> m_Meshes;
  DynamicArray<Ref<Material>> m_Materials;
};

} // namespace Neptune
