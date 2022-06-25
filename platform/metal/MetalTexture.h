#pragma once

#include "renderer/Texture.h"

#import <Metal/MTLDevice.h>
#import <Metal/MTLTexture.h>

namespace Neptune
{

// ----- MetalTexture -----------------

class MetalTexture final : public Texture
{
public:
  MetalTexture(id<MTLDevice>, const TextureDesc& desc);
  ~MetalTexture();
  
  id<MTLTexture> GetTexture() noexcept { return m_Texture; }
  
private:
  id<MTLTexture> m_Texture;
};

} // namespace Neptune
