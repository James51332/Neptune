#include "neptunepch.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <OpenFBX/ofbx.h>

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
	out.color = float4(float3(max(dot(abs(in.normal), envLightDir),0.0f)), 1);
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

ofbx::IScene* scene;

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
    std::ifstream fs("resources/gun.fbx");
    std::stringstream ss;
    ss << fs.rdbuf();
    std::string string = ss.str();
    
    const ofbx::u8* data = reinterpret_cast<const ofbx::u8*>(string.c_str());
    scene = ofbx::load(data, static_cast<int>(string.length()), static_cast<ofbx::u64>(ofbx::LoadFlags::TRIANGULATE));
  }

  {
    Int32 meshes = scene->getMeshCount();
    for (Int32 i = (meshes - 1); i >= 0; i--)
    {
      const ofbx::Mesh* mesh = scene->getMesh(static_cast<int>(i));
      
      struct Vertex
      {
        Float3 pos;
        Float3 normal;
      };
      
      const ofbx::Geometry* geo = mesh->getGeometry();
      const ofbx::Vec3* vert = geo->getVertices();
      const ofbx::Vec3* norm = geo->getNormals();
      const int cnt = geo->getVertexCount();
        
      Vertex* vertices = new Vertex[cnt];
      for (int i = 0; i < cnt; i++)
      {
        vertices[i].pos = { vert[i].x, vert[i].y, vert[i].z };
        vertices[i].normal = { norm[i].x, norm[i].y, norm[i].z };
      }
      
      BufferDesc vbDesc;
      vbDesc.Type = BufferType::Vertex;
      vbDesc.Size = cnt * sizeof(Vertex);
      vbDesc.Data = (void*)vertices;
      vbDesc.Usage = BufferUsage::Static;
      m_VB = m_RenderDevice->CreateBuffer(vbDesc);
      
      delete[] vertices;
        
      const int indexCount = geo->getIndexCount();
      const int* faceIndices = geo->getFaceIndices();
        
      UInt16* indices = new UInt16[indexCount];
      
      for (int i = 0; i < indexCount; i++)
      {
        indices[i] = faceIndices[i] >= 0 ? faceIndices[i] : ~faceIndices[i];
      }
      
      BufferDesc ibDesc;
      ibDesc.Type = BufferType::Index;
      ibDesc.Usage = BufferUsage::Static;
      ibDesc.Size = sizeof(UInt16) * indexCount;
      ibDesc.Data = (void*)indices;
      m_IB = m_RenderDevice->CreateBuffer(ibDesc);
      
      delete[] indices;
      
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
        { PipelineAttributeType::Float3, "Normal" }
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

    RenderCommand::SetVertexBuffer(m_VB, 0);
    RenderCommand::SetVertexBuffer(m_UB, 1);
    RenderCommand::SetPipelineState(m_Pipeline);
    
    DrawCommandDesc cmd;
    cmd.Type = PrimitiveType::Triangle;
    cmd.IndexBuffer = m_IB;
    cmd.Indexed = true;
    cmd.IndexType = IndexType::UInt16;
    cmd.Count = m_IB->GetSize() / sizeof(UInt16);
    cmd.Offset = 0;
    RenderCommand::Submit(cmd);
    
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
                     
