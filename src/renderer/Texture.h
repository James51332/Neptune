#pragma once

namespace Neptune
{

// ----- Texture -----------------

enum class TextureType
{
  Texture2D
};

struct TextureDesc
{
  Size Width;
  Size Height;
  bool Mipmapped;
  TextureType Type;
  void* Data;
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
