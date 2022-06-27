#include "neptunepch.h"
#include "ImGUIRenderer.h"

#include "core/KeyCode.h"
#include "core/MouseCode.h"
#include "core/EventQueue.h"
#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Neptune
{

// ----- Shader Source -----------------

static const char* ImGuiShaderSrc = R"(
#include <metal_stdlib>
using namespace metal;

struct VSInput
{
  float2 position [[attribute(0)]];
  float2 texCoord [[attribute(1)]];
  float4 color [[attribute(2)]];
};

struct FSInput
{
  float4 position [[position]];
  float2 texCoord;
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
  out.position = uniform.viewProjection * float4(in.position, 0, 1);
  out.texCoord = in.texCoord;
  out.color = in.color;
  return out;
}

fragment float4 fragmentFunc(FSInput in [[stage_in]],
                             texture2d<float> texture [[texture(0)]])
{
  constexpr sampler linearSampler(coord::normalized, min_filter::linear, mag_filter::linear, mip_filter::linear);
  float4 texColor = texture.sample(linearSampler, in.texCoord);
  return in.color * texColor;
})";

// ----- ImGUIRenderer -----------------

ImGUIRendererData ImGUIRenderer::s_Data;

void ImGUIRenderer::OnInit(const Ref<RenderDevice>& device, Size width, Size height)
{
  s_Data.Device = device;
  
  // Initialize ImGui
  {
  	ImGui::CreateContext();
  	ImGuiIO& io = ImGui::GetIO();
  	
  	io.DisplaySize.x = width;
  	io.DisplaySize.y = height;
  	
  	io.BackendPlatformName = "Neptune";
  	io.BackendRendererName = "Neptune Renderer";
  	
  	io.KeyMap[ImGuiKey_Tab] = KeyCode::KeyTab;
  	io.KeyMap[ImGuiKey_LeftArrow] = KeyCode::KeyLeft;
  	io.KeyMap[ImGuiKey_RightArrow] = KeyCode::KeyRight;
  	io.KeyMap[ImGuiKey_UpArrow] = KeyCode::KeyUp;
  	io.KeyMap[ImGuiKey_DownArrow] = KeyCode::KeyDown;
    io.KeyMap[ImGuiKey_Backspace] = KeyCode::KeyBackspace;
    io.KeyMap[ImGuiKey_Space] = KeyCode::KeySpace;
    io.KeyMap[ImGuiKey_Enter] = KeyCode::KeyEnter;
    io.KeyMap[ImGuiKey_Escape] = KeyCode::KeyEscape;
    io.KeyMap[ImGuiKey_A] = KeyCode::KeyA;
    io.KeyMap[ImGuiKey_C] = KeyCode::KeyC;
    io.KeyMap[ImGuiKey_V] = KeyCode::KeyV;
    io.KeyMap[ImGuiKey_X] = KeyCode::KeyX;
    io.KeyMap[ImGuiKey_Y] = KeyCode::KeyY;
    io.KeyMap[ImGuiKey_Z] = KeyCode::KeyZ;
  	io.KeyMap[ImGuiKey_KeyPadEnter] = KeyCode::KeyPadEnter;
  	io.KeyMap[ImGuiKey_PageUp] = KeyCode::KeyPageUp;
  	io.KeyMap[ImGuiKey_PageDown] = KeyCode::KeyPageDown;
  	io.KeyMap[ImGuiKey_Home] = KeyCode::KeyHome;
  	io.KeyMap[ImGuiKey_End] = KeyCode::KeyEnd;
  	io.KeyMap[ImGuiKey_Insert] = KeyCode::KeyInsert;
  	io.KeyMap[ImGuiKey_Delete] = KeyCode::KeyDelete;
  }
  
  // Create Buffers
  {
    BufferDesc vb;
    vb.Type = BufferType::Vertex;
    vb.Size = 500000; // 15 buffers * 0.5mb = 7.5mb (reasonable)
    vb.Usage = BufferUsage::Dynamic;
    vb.Data = nullptr;
    
    for (Size i = 0; i < s_Data.NumBuffers; ++i)
    	s_Data.VertexBuffers[i] = s_Data.Device->CreateBuffer(vb);
    
    BufferDesc ib;
    ib.Type = BufferType::Index;
    ib.Size = 100000; // 15 buffers * 0.1mb = 1.5mb (reasonable)
    ib.Usage = BufferUsage::Dynamic;
    ib.Data = nullptr;
    
    for (Size i = 0; i < s_Data.NumBuffers; ++i)
      s_Data.IndexBuffers[i] = s_Data.Device->CreateBuffer(ib);
    
    BufferDesc ub;
    ub.Type = BufferType::Uniform;
    ub.Size = sizeof(Matrix4);
    ub.Usage = BufferUsage::Dynamic;
    ub.Data = nullptr;
    s_Data.UniformBuffer = s_Data.Device->CreateBuffer(ub);
  }
  
  // Create Pipeline State
  {
    ShaderDesc sd;
    sd.vertexSrc = ImGuiShaderSrc;
    sd.fragmentSrc = ImGuiShaderSrc;
    Ref<Shader> shader = s_Data.Device->CreateShader(sd);
    
    DepthStencilState depth;
    depth.DepthWriteEnabled = false;
    depth.Function = CompareFunction::Always;
    
    PipelineStateDesc desc;
    desc.Shader = shader;
    desc.Layout = {
      { PipelineAttributeType::Float2, "Position" },
      { PipelineAttributeType::Float2, "TexCoord" },
      { PipelineAttributeType::UChar4, "Color", true }
    };
    desc.DepthStencilState = depth;
    
    s_Data.PipelineState = s_Data.Device->CreatePipelineState(desc);
  }
  
  // Create Font Texture
  {
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    
    TextureDesc desc;
    desc.Type = TextureType::Texture2D;
    desc.PixelFormat = PixelFormat::RGBA8Unorm;
    desc.Data = pixels;
    desc.RenderTarget = false;
    desc.Width = (Size)width;
    desc.Height = (Size)height;
    desc.Mipmapped = false;
    s_Data.FontTexture = s_Data.Device->CreateTexture(desc);
  }
}

void ImGUIRenderer::OnUpdate()
{
  ImGui::NewFrame();
}

void ImGUIRenderer::OnTerminate()
{
  ImGui::DestroyContext();
}

void ImGUIRenderer::Render(const Ref<Framebuffer> &framebuffer)
{
  ImGui::Render();
  ImDrawData* drawData = ImGui::GetDrawData();
  
  Size framebufferWidth = (Size)(drawData->DisplaySize.x * drawData->FramebufferScale.x);
  Size framebufferHeight = (Size)(drawData->DisplaySize.y * drawData->FramebufferScale.y);
  if (framebufferWidth <= 0 || framebufferHeight <= 0 || drawData->CmdListsCount == 0) return;
  
  // Update Uniforms
  {
    Float32 L = drawData->DisplayPos.x;
    Float32 R = drawData->DisplayPos.x + drawData->DisplaySize.x;
    Float32 T = drawData->DisplayPos.y;
    Float32 B = drawData->DisplayPos.y + drawData->DisplaySize.y;
    
    Matrix4 viewProj = glm::ortho(L, R, B, T);
    s_Data.UniformBuffer->Update(sizeof(Matrix4), &viewProj[0][0]);
  }
  
  // Begin Recording Commandbuffer
  {
    CommandBuffer commandBuffer;
    RenderCommand::BeginRecording(commandBuffer);
    {
      RenderPass renderPass;
      {
        renderPass.ClearColor = { 0.1f, 0.1f, 0.1f, 1.0f };
        renderPass.LoadAction = LoadAction::Clear;
        renderPass.StoreAction = StoreAction::Store;
        renderPass.Framebuffer = framebuffer;
      }
      RenderCommand::BeginRenderPass(renderPass);
      
      // Will project scissor/clipping rectangles into framebuffer space
      ImVec2 clipOff = drawData->DisplayPos;         // (0,0) unless using multi-viewports
      ImVec2 clipScale = drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

      for (Size i = 0; i < drawData->CmdListsCount; i++)
      {
        const ImDrawList* commandList = drawData->CmdLists[i];
        
        s_Data.NextBuffer = (s_Data.NextBuffer + 1) % s_Data.NumBuffers;
        s_Data.VertexBuffers[s_Data.NextBuffer]->Update(commandList->VtxBuffer.Size * sizeof(ImDrawVert), commandList->VtxBuffer.Data);
        s_Data.IndexBuffers[s_Data.NextBuffer]->Update(commandList->IdxBuffer.Size * sizeof(ImDrawIdx), commandList->IdxBuffer.Data);
        
        for (Size cmd_i = 0; cmd_i < commandList->CmdBuffer.Size; cmd_i++)
        {
          const ImDrawCmd* cmd = &commandList->CmdBuffer[(int)cmd_i];
          
          // Project scissor/clipping rectangles into framebuffer space
          ImVec2 clipMin((cmd->ClipRect.x - clipOff.x) * clipScale.x, (cmd->ClipRect.y - clipOff.y) * clipScale.y);
          ImVec2 clipMax((cmd->ClipRect.z - clipOff.x) * clipScale.x, (cmd->ClipRect.w - clipOff.y) * clipScale.y);
          
          // Clamp to viewport as setScissorRect() won't accept values that are off bounds
          if (clipMin.x < 0.0f) { clipMin.x = 0.0f; }
          if (clipMin.y < 0.0f) { clipMin.y = 0.0f; }
          if (clipMax.x > framebufferWidth) { clipMax.x = (float)framebufferWidth; }
          if (clipMax.y > framebufferHeight) { clipMax.y = (float)framebufferHeight; }
          if (clipMax.x <= clipMin.x || clipMax.y <= clipMin.y)
            continue;
          if (cmd->ElemCount == 0) // drawIndexedPrimitives() validation doesn't accept this
            continue;

          RenderCommand::SetClipRect(clipMin.x, clipMin.y, clipMax.x - clipMin.x, clipMax.y - clipMin.y);
          RenderCommand::SetPipelineState(s_Data.PipelineState);
          RenderCommand::SetVertexBuffer(s_Data.VertexBuffers[s_Data.NextBuffer], 0);
          RenderCommand::SetVertexBuffer(s_Data.UniformBuffer, 1);
          RenderCommand::BindTexture(s_Data.FontTexture, 0);
          
          DrawCommandDesc drawCmd;
          {
          	drawCmd.Type = PrimitiveType::Triangle;
          	drawCmd.Indexed = true;
            drawCmd.IndexBuffer = s_Data.IndexBuffers[s_Data.NextBuffer];
          	drawCmd.Count = cmd->ElemCount;
          	drawCmd.IndexType = sizeof(ImDrawIdx) == 2 ? IndexType::UInt16 : IndexType::UInt32;
          	drawCmd.Offset = cmd->IdxOffset * sizeof(ImDrawIdx);
          }
          RenderCommand::Submit(drawCmd);
        }
      }
      RenderCommand::EndRenderPass();
    }
    RenderCommand::EndRecording();
    s_Data.Device->Submit(commandBuffer);
  }
}

void ImGUIRenderer::OnEvent(Scope<Event> &e)
{  
  EventQueue::Dispatch<WindowResizedEvent>(e, [](const WindowResizedEvent& event)
	{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = event.GetWidth();
    io.DisplaySize.y = event.GetHeight();
    return false;
  });
  
  EventQueue::Dispatch<KeyTypedEvent>(e, [](const KeyTypedEvent& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    io.AddInputCharacter(event.GetKeyCode());
    return false;
  });
  
  EventQueue::Dispatch<KeyPressedEvent>(e, [](const KeyPressedEvent& event)
	{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[event.GetKeyCode()] = true;
    return false;
  });
  
  EventQueue::Dispatch<KeyReleasedEvent>(e, [](const KeyReleasedEvent& event)
                                        {
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[event.GetKeyCode()] = false;
    return false;
  });
  
  EventQueue::Dispatch<MousePressedEvent>(e, [](const MousePressedEvent& event)
	{
    ImGuiIO& io = ImGui::GetIO();
    if (event.GetMouseCode() <= 5)
    	io.MouseDown[event.GetMouseCode() - 1] = true;
    return false;
  });
  
  EventQueue::Dispatch<MouseReleasedEvent>(e, [](const MouseReleasedEvent& event)
	{
    ImGuiIO& io = ImGui::GetIO();
    if (event.GetMouseCode() <= 5)
    	io.MouseDown[event.GetMouseCode() - 1] = false;
    return false;
  });
  
  EventQueue::Dispatch<MouseMovedEvent>(e, [](const MouseMovedEvent& event)
	{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos.x = event.GetXPosition();
    io.MousePos.y = event.GetYPosition();
    return false;
  });
  
  EventQueue::Dispatch<MouseScrolledEvent>(e, [](const MouseScrolledEvent& event)
  {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseWheel += event.GetYScroll();
    io.MouseWheelH += event.GetXScroll();
    return false;
  });
}

} // namespace Neptune
