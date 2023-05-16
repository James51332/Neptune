#pragma once

#include "Buffer.h"

namespace Neptune
{

// ----- MeshVertex -----------------

struct MeshVertex
{
  Float3 Position;
  Float3 Normal;
  Float4 Color;
  Float2 UV;
};

using MeshIndex = UInt32;

// ----- Mesh -----------------

struct MeshDesc
{
  MeshVertex* Vertices = nullptr;
  Size NumVertices;
  
  MeshIndex* Indices = nullptr;
  Size NumIndices;
};

// This is the handle to the renderer for the mesh. For now,
// each mesh will have its own vertex and index buffer. A material will
// probably be representative of a pipeline state and uniforms. We won't
// worry about submeshes or meshes with multiple materials for the time being.
class Mesh
{
  friend class MetalRenderDevice;
  
  friend class RenderCommand;
public:
  ~Mesh() = default;
  
protected:
  Mesh(const Ref<Buffer>& vb, const Ref<Buffer>& ib);
  
private:
  Ref<Buffer> m_VertexBuffer;
  Ref<Buffer> m_IndexBuffer;
};

} // namespace Neptune
