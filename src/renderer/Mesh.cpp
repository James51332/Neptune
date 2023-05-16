#include "neptunepch.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Neptune
{

// ----- Mesh -----------------

Mesh::Mesh(const Ref<Buffer>& vb, const Ref<Buffer>& ib)
	: m_VertexBuffer(vb), m_IndexBuffer(ib)
{
}

} // namespace Neptune
