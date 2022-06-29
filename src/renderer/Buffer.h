#pragma once

namespace Neptune
{

// ----- Buffer -----------------

enum class BufferType
{
  Vertex,
  Index,
  Uniform
};

enum class BufferUsage
{
  Static,
  Dynamic
};

struct BufferDesc
{
  Size Size;
  BufferType Type;
  BufferUsage Usage;
  void* Data;
};

class Buffer
{
public:
  virtual ~Buffer() = default;
  
  Size GetSize() const noexcept { return m_Size; }
  BufferUsage GetUsage() const noexcept { return m_Usage; }
  BufferType GetType() const noexcept { return m_Type; }
  
  // For dynamic buffers, updates the contents. Dynamic buffers are triple-backed, so even
  // with multiple frames-in-flight, no gpu synchronization is need for this operation
  virtual void Update(Size size, const void* data) = 0;
  
protected:
  Buffer(Size size, BufferUsage usage, BufferType type)
  	: m_Size(size), m_Usage(usage), m_Type(type) {}
  
  Size m_Size;
  BufferUsage m_Usage;
  BufferType m_Type;
};

} // namespace Neptune
