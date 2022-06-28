#pragma once

namespace Neptune
{

// ----- Texture -----------------

enum class TextureType
{
  Texture2D
};

enum class PixelFormat
{
  RGBA8Unorm,
  BGRA8Unorm,
  Depth32
};

struct TextureDesc
{
  Size Width;
  Size Height;
  PixelFormat PixelFormat;
  bool Mipmapped;
  TextureType Type;
  void* Data;
  bool RenderTarget = false;
};

class Texture
{
public:
  virtual ~Texture() = default;
  
  const TextureDesc& GetDesc() const noexcept { return m_Desc; }
  
protected:
  Texture(const TextureDesc& desc)
  	: m_Desc(desc) {}
  
  TextureDesc m_Desc;
};

} // namespace Neptune
