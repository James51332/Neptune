#include "neptunepch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>
#include <fstream>
#include <sstream>

#define GLM_DEPTH_FORCE_ONE_TO_ZERO
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{

const char* meshSrc = R"(
#include <metal_stdlib>
using namespace metal;

struct VSInput
{
  float3 position [[attribute(0)]];
 	float3 normal [[attribute(1)]];
	float4 color [[attribute(2)]];
	float2 uv [[attribute(3)]];
};

struct FSInput
{
  float4 position [[position]];
 	float4 color;
};

struct Uniform
{
  float4x4 viewProjection;
};

vertex FSInput vertexFunc(VSInput in [[stage_in]],
                        constant Uniform& uniform [[buffer(1)]])
{
  float3 envLightDir(0, 1, 0);

  FSInput out;
  out.position = uniform.viewProjection * float4(in.position , 1);
 	out.color = in.color * float4(float3(max(dot(abs(in.normal), envLightDir), 0.0f)), 1);
  return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]])
{
  return in.color;
})";

EditorLayer::EditorLayer()
: Layer("Editor Layer")
{
  
}

EditorLayer::~EditorLayer()
{
  
}

void EditorLayer::OnInit(const Ref<RenderDevice>& device)
{
  m_RenderDevice = device;
  
  // Create framebuffers
  {
    FramebufferDesc desc;
    desc.Width = m_ViewportSize.x;
    desc.Height = m_ViewportSize.y;
    
    for (Size i = 0; i < Renderer::GetMaxFramesInFlight(); i++)
      m_Framebuffers.PushBack(m_RenderDevice->CreateFramebuffer(desc));
  }
  
  // Load Texture
  m_Texture = m_RenderDevice->LoadTexture("resources/panda.png");
  
  // Create Camera
  {
    CameraDesc desc;
    desc.Type = ProjectionType::Perspective;
    desc.Position = { 0.0f, 0.0f, 4.0f };
    desc.Rotation = { 0.0f, 0.0f, 0.0f };
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    desc.Near = 0.1f;
    desc.Far = 100.0f;
    desc.FOV = 45;
    Camera cam = Camera(desc);
    m_CameraController = CameraController(cam);
  }
  
  // Try load a model or sum
  {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("resources/gun.fbx", aiProcess_Triangulate | aiProcess_GenNormals);
    
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
      NEPTUNE_ERROR("Failed to open scene!");
    }
    
    aiMesh* mesh = scene->mMeshes[3];
    
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
    m_Mesh = m_RenderDevice->CreateMesh(desc);
  }
  
  {
    BufferDesc ubDesc;
    ubDesc.Type = BufferType::Uniform;
    ubDesc.Usage = BufferUsage::Dynamic;
    ubDesc.Size = sizeof(Matrix4);
    ubDesc.Data = (void*)&m_CameraController.GetCamera().GetViewProjectionMatrix()[0][0];
    m_UB = m_RenderDevice->CreateBuffer(ubDesc);
  }
  
  {
    PipelineStateDesc desc;
    
    desc.Layout = {
      { PipelineAttributeType::Float3, "Position" },
      { PipelineAttributeType::Float3, "Normal" },
      { PipelineAttributeType::Float4, "Color" },
      { PipelineAttributeType::Float2, "UV" }
    };
    
    DepthStencilState dss;
    dss.DepthWriteEnabled = true;
    dss.Function = CompareFunction::Less;
    desc.DepthStencilState = dss;
    
    ShaderDesc s;
    s.vertexSrc = meshSrc;
    s.fragmentSrc = meshSrc;
    desc.Shader = m_RenderDevice->CreateShader(s);
    
    m_Pipeline = m_RenderDevice->CreatePipelineState(desc);
  }
}

void EditorLayer::OnTerminate()
{
}

void EditorLayer::OnUpdate()
{
  m_CameraController.OnUpdate();
  m_UB->Update(sizeof(Matrix4), &m_CameraController.GetCamera().GetViewProjectionMatrix()[0][0]);
  
  // Resize Framebuffer
  {
    FramebufferDesc desc = m_Framebuffers[Renderer::GetFrameNumber()]->GetDesc();
    if (desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y)
      m_Framebuffers[Renderer::GetFrameNumber()]->Resize(m_ViewportSize.x, m_ViewportSize.y);
  }
}

void EditorLayer::OnRender(const Ref<Framebuffer>& framebuffer)
{
  // Scene Pass
  {
    RenderPass scenePass;
    {
      scenePass.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
      scenePass.LoadAction = LoadAction::Clear;
      scenePass.StoreAction = StoreAction::Store;
      scenePass.Framebuffer = m_Framebuffers[Renderer::GetFrameNumber()];
    }
    RenderCommand::BeginRenderPass(scenePass);
    
    RenderCommand::SetVertexBuffer(m_UB, 1);
    RenderCommand::SetPipelineState(m_Pipeline);
    RenderCommand::BindTexture(m_Texture, 0);
    RenderCommand::Submit(m_Mesh);
    
    RenderCommand::EndRenderPass();
  }
  
  // ImGui (Swapchain Pass)
  {
    RenderPass renderPass;
    {
      renderPass.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
      renderPass.LoadAction = LoadAction::Clear;
      renderPass.StoreAction = StoreAction::Store;
      renderPass.Framebuffer = framebuffer;
    }
    RenderCommand::BeginRenderPass(renderPass);
    
    ImGUIRenderer::Render();
    
    RenderCommand::EndRenderPass();
  }
}

void EditorLayer::OnImGuiRender()
{
  // Viewport
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
    ImGui::Begin("Viewport");
    
    bool block = !ImGui::IsWindowFocused() && !ImGui::IsWindowHovered();
    ImGUIRenderer::BlockEvents(block);
    
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)&m_Framebuffers[Renderer::GetFrameNumber()]->GetColorAttachment(), ImVec2(m_ViewportSize.x, m_ViewportSize.y));
    m_ViewportSize = { viewSize.x, viewSize.y };
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
}

void EditorLayer::OnEvent(Scope<Event>& e)
{
  
}

} // namespace Neptune

