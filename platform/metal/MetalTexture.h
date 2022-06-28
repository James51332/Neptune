#pragma once

#include "renderer/Texture.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLTexture.h>
#import <QuartzCore/CAMetalLayer.h>

namespace Neptune
{

// ----- MetalTexture -----------------

class MetalTexture : public Texture
{
public:
  MetalTexture(id<MTLDevice>, const TextureDesc& desc);
  virtual ~MetalTexture();
  
  virtual id<MTLTexture> GetTexture() { return m_Texture; }
  
protected:
  MetalTexture(const TextureDesc& desc)
  	: Texture(desc) {}
  
  id<MTLTexture> m_Texture = nil;
};

// ----- MetalSwapchainTexture -----------------

// Represents a CAMetalDrawable and the backing texture.
class MetalSwapchainTexture final : public MetalTexture
{
public:
  MetalSwapchainTexture(id<MTLDevice> device, CAMetalLayer* layer, const TextureDesc& desc);
  ~MetalSwapchainTexture();
  
  id<CAMetalDrawable> GetDrawable();
  void Present();
  
  virtual id<MTLTexture> GetTexture() override { return GetDrawable().texture; }
  bool GetAvailability() { return m_Available; }
  
private:
  CAMetalLayer* m_Layer;
  Atomic<bool> m_Available;
  
  id<CAMetalDrawable> m_Drawable;
};

} // namespace Neptune
