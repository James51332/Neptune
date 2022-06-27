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
  MetalFramebuffer(id<MTLDevice> device, CAMetalLayer* layer, Size width, Size height);
  ~MetalFramebuffer();
  
  id<CAMetalDrawable> GetDrawable();
  id<MTLDepthStencilState> GetDepthStencilState() { return m_DepthStencil; }
  void Present();
  
  void Resize(Size width, Size height);
  
private:
  void CreateDepthTexture();
  
private:
  CAMetalLayer* m_Layer = nil;
  id<CAMetalDrawable> m_Drawable = nil;
  id<MTLDepthStencilState> m_DepthStencil = nil;
  
  Size m_Width, m_Height;
  
  bool m_Available = true;
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
