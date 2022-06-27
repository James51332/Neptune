#include "neptunepch.h"
#include "MetalTexture.h"

namespace Neptune
{

// ----- MetalTexture -----------------

static MTLPixelFormat MTLPixelFormatFromPixelFormat(PixelFormat format)
{
  switch (format)
  {
    case PixelFormat::RGBA8Unorm: return MTLPixelFormatRGBA8Unorm;
    case PixelFormat::Depth32: return MTLPixelFormatDepth32Float;
  }
  
  return MTLPixelFormatInvalid;
}

MetalTexture::MetalTexture(id<MTLDevice> device, const TextureDesc& desc)
	: Texture(desc), m_Texture(nullptr)
{
  @autoreleasepool
  {
    MTLTextureDescriptor* descriptor;
    switch (desc.Type)
    {
      case TextureType::Texture2D:
      {
      	descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat: MTLPixelFormatFromPixelFormat(desc.PixelFormat) 																																width: desc.Width
      	                                                               height: desc.Height
      	                                                            mipmapped: desc.Mipmapped];
        
        if (desc.RenderTarget)
        	descriptor.usage = MTLTextureUsageRenderTarget;
        
        break;
    	}
      default: NEPTUNE_ASSERT(false, "Failed to create MTLTextureDescriptor!");
    }
    
    m_Texture = [device newTextureWithDescriptor: descriptor];
    NEPTUNE_ASSERT(m_Texture, "Failed to create MTLTexture!");
    
    if (!desc.Data) return;
    
    MTLRegion region = {
      { 0, 0, 0 },
      { desc.Width, desc.Height, 1 }
    };
    
    [m_Texture replaceRegion: region
                 mipmapLevel: 0
                   withBytes: desc.Data
                 bytesPerRow: 4 * desc.Width];
  }
}

MetalTexture::~MetalTexture()
{
  @autoreleasepool
  {
    [m_Texture release];
    m_Texture = nil;
  }
}

} // namespace Neptune
