#include "neptunepch.h"
#include "Model.h"

#include "renderer/RenderDevice.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Neptune
{

// ----- Model -----------------

static void processMesh(RenderDevice* device, DynamicArray<Ref<Mesh>>& meshes, aiMesh* mesh, const aiScene* scene)
{
  MeshDesc desc;
  desc.NumVertices = mesh->mNumVertices;
  DynamicArray<MeshVertex> vertices;
  DynamicArray<MeshIndex> indices;
  
  for (Size i = 0; i < desc.NumVertices; i++)
  {
    MeshVertex vertex;
    
    Float3 position;
    position.x = mesh->mVertices[i].x;
    position.y = mesh->mVertices[i].y;
    position.z = mesh->mVertices[i].z;
    vertex.Position = position;
    
    Float3 normal;
    normal.x = mesh->mNormals[i].x;
    normal.y = mesh->mNormals[i].y;
    normal.z = mesh->mNormals[i].z;
    vertex.Normal = normal;
    
    Float4 color = Float4(1.0);
    if (mesh->mColors[0])
    {
      color.r = mesh->mColors[i]->r;
      color.g = mesh->mColors[i]->g;
      color.b = mesh->mColors[i]->b;
      color.a = mesh->mColors[i]->a;
    }
    vertex.Color = color;
    
    Float2 uv = Float2(0.0f);
    if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
    {
      uv.x = mesh->mTextureCoords[0][i].x;
      uv.y = mesh->mTextureCoords[0][i].y;
    }
    vertex.UV = uv;
    
    vertices.PushBack(vertex);
  }
  
  desc.Vertices = &vertices[0];
  
  for(Size i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for(Size j = 0; j < face.mNumIndices; j++)
      indices.PushBack(face.mIndices[j]);
  }
  
  desc.Indices = &indices[0];
  desc.NumIndices = indices.Size();
  meshes.PushBack(device->CreateMesh(desc));
}

static void processNode(RenderDevice* device, DynamicArray<Ref<Mesh>>& meshes, aiNode* node, const aiScene* scene)
{
  for (Size i = 0; i < node->mNumMeshes; i++)
  {
    processMesh(device, meshes, scene->mMeshes[node->mMeshes[i]], scene);
  }
  
  for (Size i = 0; i < node->mNumChildren; i++)
  {
    processNode(device, meshes, node->mChildren[i], scene);
  }
}

Model::Model(RenderDevice* device, const ModelDesc& desc)
{
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile("resources/Car.obj", aiProcess_Triangulate | aiProcess_GenNormals);
  
  if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    NEPTUNE_ERROR("Failed to open scene!");
  
  
  aiNode* node = scene->mRootNode;
  processNode(device, m_Meshes, node, scene);

  MaterialDesc md;
  md.Shininess = 0.5f;
  md.Specular = Float4(1.0f, 1.0f, 1.0f, 1.0f);
  md.Diffuse = Float4(1.0f, 0.2f, 0.8f, 1.0f);
  md.Ambient = Float4(1.0f, 0.2f, 0.8f, 1.0f);
  m_Materials.PushBack(device->CreateMaterial(md));
}

} // namespace Neptune
