#pragma once

namespace Neptune
{

// ----- Framebuffer -----------------

class Framebuffer
{
public:
  virtual ~Framebuffer() = default;
};

// ----- Swapchain -----------------

// A swapchain is a collection of images which can be presented to the display. Our
// API doesn't need to handle the complexities of Vulkan, but we want to have the option
// to have multiple frames-in-flight (encoded sequentially).
class Swapchain
{
public:
  virtual ~Swapchain() = default;
  
  // This may block. If all framebuffers are in use, the render thread really doesn't
  // need to be doing anything anyways.
  virtual Ref<Framebuffer> GetNextFramebuffer() noexcept = 0;
  virtual void Present(const Ref<Framebuffer>& framebuffer) = 0;
};

} // namespace Neptune
