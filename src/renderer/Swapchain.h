#pragma once

#include "Texture.h"

namespace Neptune
{

// ----- Framebuffer -----------------

struct FramebufferDesc
{
  Size Width;
  Size Height;
};

class Framebuffer
{
public:
  virtual ~Framebuffer() = default;
  
  const Ref<Texture>& GetColorAttachment() const noexcept { return m_ColorTexture; }
  const Ref<Texture>& GetDepthAttachment() const noexcept { return m_DepthTexture; }
  
  virtual void Resize(Size width, Size height) = 0;
  
protected:
  Ref<Texture> m_ColorTexture;
  Ref<Texture> m_DepthTexture;
};

// ----- Swapchain -----------------

// A swapchain is a collection of images which can be presented to the display. Our
// API doesn't need to handle the complexities of Vulkan, but we want to have the option
// to have multiple frames-in-flight (encoded sequentially).
class Swapchain
{
public:
  virtual ~Swapchain() = default;
  
  virtual void Resize(Size width, Size height) = 0;
  
  // This may block. If all framebuffers are in use, the render thread really doesn't
  // need to be doing anything anyways.
  virtual Ref<Framebuffer> GetNextFramebuffer() noexcept = 0;
  virtual void Present(const Ref<Framebuffer>& framebuffer) = 0;
};

} // namespace Neptune
