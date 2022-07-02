#include "neptunepch.h"
#include "SandboxLayer.h"

#include <imgui/imgui.h>
#include <OpenFBX/ofbx.h>

#include <iostream>
#include <fstream>
#include <sstream>

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
  FSInput out;
  out.position = uniform.viewProjection * float4(in.position , 1);
	out.color = float4(abs(in.normal), 1);
  return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]])
{
  return in.color;
})";

SandboxLayer::SandboxLayer()
	: Layer("Sandbox Layer")
{
  
}

SandboxLayer::~SandboxLayer()
{
  
}

ofbx::IScene* scene;

void SandboxLayer::OnInit(const Neptune::Ref<Neptune::RenderDevice>& device)
{
  m_RenderDevice = device;
  
  // Create framebuffers
  {
    Neptune::FramebufferDesc desc;
    desc.Width = m_ViewportSize.x;
    desc.Height = m_ViewportSize.y;
    
    for (Neptune::Size i = 0; i < Neptune::Renderer::GetMaxFramesInFlight(); i++)
      m_Framebuffers.PushBack(m_RenderDevice->CreateFramebuffer(desc));
  }
  
  // Load Texture
  m_Texture = m_RenderDevice->LoadTexture("resources/panda.png");
  
  // Create Camera
  {
    Neptune::CameraDesc desc;
    desc.Type = Neptune::ProjectionType::Perspective;
    desc.Position = { 0.0f, 0.0f, 4.0f };
    desc.Rotation = { -90.0f, 0.0f, 0.0f };
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    desc.Near = 0.1f;
    desc.Far = 100.0f;
    desc.FOV = 45;
    m_Camera = Neptune::Camera(desc);
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
        Neptune::Float3 pos;
        Neptune::Float3 normal;
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
      
      Neptune::BufferDesc vbDesc;
      vbDesc.Type = Neptune::BufferType::Vertex;
      vbDesc.Size = cnt * sizeof(Vertex);
      vbDesc.Data = (void*)vertices;
      vbDesc.Usage = Neptune::BufferUsage::Static;
      m_VB = m_RenderDevice->CreateBuffer(vbDesc);
      
      delete[] vertices;
        
      const int indexCount = geo->getIndexCount();
      const int* faceIndices = geo->getFaceIndices();
        
      UInt16* indices = new UInt16[indexCount];
      
      for (int i = 0; i < indexCount; i++)
      {
        indices[i] = faceIndices[i] >= 0 ? faceIndices[i] : ~faceIndices[i];
      }
      
      Neptune::BufferDesc ibDesc;
      ibDesc.Type = Neptune::BufferType::Index;
      ibDesc.Usage = Neptune::BufferUsage::Static;
      ibDesc.Size = sizeof(UInt16) * indexCount;
      ibDesc.Data = (void*)indices;
      m_IB = m_RenderDevice->CreateBuffer(ibDesc);
      
      delete[] indices;
      
      Neptune::BufferDesc ubDesc;
      ubDesc.Type = Neptune::BufferType::Uniform;
      ubDesc.Usage = Neptune::BufferUsage::Dynamic;
      ubDesc.Size = sizeof(Neptune::Matrix4);
      ubDesc.Data = (void*)&m_Camera.GetViewProjectionMatrix()[0][0];
      m_UB = m_RenderDevice->CreateBuffer(ubDesc);
    }
    
    {
      Neptune::PipelineStateDesc desc;
      
      desc.Layout = {
        { Neptune::PipelineAttributeType::Float3, "Position" },
        { Neptune::PipelineAttributeType::Float3, "Normal" }
      };
      
      Neptune::DepthStencilState dss;
      dss.DepthWriteEnabled = true;
      dss.Function = Neptune::CompareFunction::Less;
      
      desc.DepthStencilState = dss;
      
      Neptune::ShaderDesc s;
      s.vertexSrc = pbrSrc;
      s.fragmentSrc = pbrSrc;
      desc.Shader = m_RenderDevice->CreateShader(s);
      
      m_Pipeline = m_RenderDevice->CreatePipelineState(desc);
    }
  }
  
}

void SandboxLayer::OnTerminate()
{
  
}

void SandboxLayer::OnUpdate()
{
  // TODO: Camera Controller
  // Update Camera
  {
    Neptune::Float3 translate = Neptune::Float3(0.0f);
    
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyD)) translate.x += 0.05f;
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyA)) translate.x -= 0.05f;
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyW)) translate.y += 0.05f;
    if (Neptune::Input::KeyDown(Neptune::KeyCode::KeyS)) translate.y -= 0.05f;
    
    Neptune::CameraDesc desc = m_Camera.GetDesc();
    desc.Position += translate;
    desc.Aspect = m_ViewportSize.x / m_ViewportSize.y;
    m_Camera.SetDesc(desc);
    
    m_UB->Update(sizeof(Neptune::Matrix4), &m_Camera.GetViewProjectionMatrix()[0][0]);
  }
  
  // Resize Framebuffer
  {
    Neptune::FramebufferDesc desc = m_Framebuffers[Neptune::Renderer::GetFrameNumber()]->GetDesc();
    if (desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y)
      m_Framebuffers[Neptune::Renderer::GetFrameNumber()]->Resize(m_ViewportSize.x, m_ViewportSize.y);
  }
}

void SandboxLayer::OnRender(const Neptune::Ref<Neptune::Framebuffer>& framebuffer)
{
  // Scene Pass
  {
    Neptune::RenderPass scenePass;
    {
      scenePass.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
      scenePass.LoadAction = Neptune::LoadAction::Clear;
      scenePass.StoreAction = Neptune::StoreAction::Store;
      scenePass.Framebuffer = m_Framebuffers[Neptune::Renderer::GetFrameNumber()];
    }
    Neptune::RenderCommand::BeginRenderPass(scenePass);
    
    //Neptune::Renderer2D::Begin(m_Camera);
    //Neptune::Renderer2D::DrawQuad(Neptune::Matrix4(1.0f), m_Texture);
    //Neptune::Renderer2D::End();
    
    Neptune::RenderCommand::SetVertexBuffer(m_VB, 0);
    Neptune::RenderCommand::SetVertexBuffer(m_UB, 1);
    Neptune::RenderCommand::SetPipelineState(m_Pipeline);
    
    Neptune::DrawCommandDesc cmd;
    cmd.Type = Neptune::PrimitiveType::Triangle;
    cmd.IndexBuffer = m_IB;
    cmd.Indexed = true;
    cmd.IndexType = Neptune::IndexType::UInt16;
    cmd.Count = m_IB->GetSize() / sizeof(UInt16);
    cmd.Offset = 0;
    Neptune::RenderCommand::Submit(cmd);
    
    Neptune::RenderCommand::EndRenderPass();
  }
  
  // ImGui (Swapchain Pass)
  {
    Neptune::RenderPass renderPass;
    {
      renderPass.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
      renderPass.LoadAction = Neptune::LoadAction::Clear;
      renderPass.StoreAction = Neptune::StoreAction::Store;
      renderPass.Framebuffer = framebuffer;
    }
    Neptune::RenderCommand::BeginRenderPass(renderPass);
    
    Neptune::ImGUIRenderer::Render();
    
    Neptune::RenderCommand::EndRenderPass();
  }
}

void SandboxLayer::OnImGuiRender()
{
  // Viewport
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
    ImGui::Begin("Viewport");
    
    ImVec2 viewSize = ImGui::GetContentRegionAvail();
    ImGui::Image((void*)&m_Framebuffers[Neptune::Renderer::GetFrameNumber()]->GetColorAttachment(), ImVec2(m_ViewportSize.x, m_ViewportSize.y));
    m_ViewportSize = { viewSize.x, viewSize.y };
    
    ImGui::End();
    ImGui::PopStyleVar();
  }
  
  // OFBX
  {
    ImGui::Begin("OFBX");
    
    ImGui::Separator();
    
    Neptune::Size meshes = scene->getMeshCount();
    for (Neptune::Size i = 0; i < meshes; ++i)
    {
      const ofbx::Mesh* mesh = scene->getMesh(static_cast<int>(i));
    	
      char label[128];
      std::memcpy(label, mesh->name, 128);
      
      ImGui::PushID(mesh->name);
      if (ImGui::TreeNode("mesh", "%s", label))
      {
        const ofbx::Geometry* geo = mesh->getGeometry();
        const ofbx::Vec3* vert = geo->getVertices();
        const int cnt = geo->getVertexCount();
        
        
        ImGui::Text("Vertices: (%i)", cnt);
        for (int i = 0; i < cnt; i++)
        {
          ImGui::Text("X: %f Y: %f Z: %f", vert[i].x, vert[i].y, vert[i].z);
        }
        
        const int indexCount = geo->getIndexCount();
        const int* faceIndices = geo->getFaceIndices();
        
        ImGui::Spacing();
        
        for (int i = 0; i < indexCount; i++)
        {
          ImGui::Text("%i", faceIndices[i] >= 0 ? faceIndices[i] : (~faceIndices[i]));
        }
        
        ImGui::TreePop();
      }
      ImGui::PopID();
    }
    
    ImGui::End();
  }
}

void SandboxLayer::OnEvent(Neptune::Scope<Neptune::Event>& e)
{
  
}
                     
