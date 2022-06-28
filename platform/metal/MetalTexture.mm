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
    case PixelFormat::BGRA8Unorm: return MTLPixelFormatBGRA8Unorm;
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
        	descriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        
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

// ----- MetalSwapchainTexture -----------------

MetalSwapchainTexture::MetalSwapchainTexture(id<MTLDevice> device, CAMetalLayer* layer, const TextureDesc& desc)
	: m_Layer([layer retain]), m_Available(true), m_Drawable(nil), MetalTexture(desc)
{
}

MetalSwapchainTexture::~MetalSwapchainTexture()
{
  @autoreleasepool
  {
    [m_Layer release];
    m_Layer = nil;
    
    [m_Drawable release];
    m_Drawable = nil;
  }
}

id<CAMetalDrawable> MetalSwapchainTexture::GetDrawable()
{
  @autoreleasepool
  {
    if (m_Available)
    {
      m_Drawable = [[m_Layer nextDrawable] retain];
      m_Available = false;
    }
    
    return m_Drawable;
  }
}

void MetalSwapchainTexture::Present()
{
  @autoreleasepool
  {
    [m_Drawable present];
    
    [m_Drawable release];
    m_Drawable = nil;
    m_Available = true;
  }
}

} // namespace Neptune
