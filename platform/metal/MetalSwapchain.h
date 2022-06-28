#pragma once
#include "renderer/Swapchain.h"

#import <QuartzCore/CAMetalLayer.h>
#import <Metal/MTLDevice.h>
#import <Metal/MTLDepthStencil.h>

namespace Neptune
{

// ----- MetalFramebuffer -----------------

class MetalFramebuffer final : public Framebuffer
{
  friend class MetalSwapchain;
  
public:
  MetalFramebuffer(id<MTLDevice> device, const FramebufferDesc& desc);
  ~MetalFramebuffer();
  
  void Resize(Size width, Size height);
  Size GetWidth() { return m_Width; }
  Size GetHeight() { return m_Height; }
  
private:
  MetalFramebuffer(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height);
  
  void CreateColorTexture();
  void CreateDepthTexture();
  
private:
  id<MTLDevice> m_Device;
  Size m_Width, m_Height;
  bool m_InSwapchain;
};

// ----- MetalSwapchain -----------------

class MetalSwapchain final : public Swapchain
{
public:
  MetalSwapchain(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height);
  ~MetalSwapchain();
  
  void Resize(Size width, Size height);
  
  Ref<Framebuffer> GetNextFramebuffer() noexcept;
  void Present(const Ref<Framebuffer>& framebuffer);
  
private:
  CAMetalLayer* m_Layer;
  Size m_Next = 0;
  
  DynamicArray<Ref<MetalFramebuffer>> m_Framebuffers;
};

} // namespace Neptune
