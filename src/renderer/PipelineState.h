#pragma once

#include "Shader.h"

namespace Neptune
{

// ----- PipelineLayout -----------------

enum class PipelineAttributeType
{
  Float,
  Float2,
  Float3,
  Float4,
  Int,
  Int2,
  Int3,
  Int4,
  Char,
  Char2,
  Char3,
  Char4,
  UInt,
  UInt2,
  UInt3,
  UInt4,
  UChar,
  UChar2,
  UChar3,
  UChar4
};

static Size PipelineAttributeTypeSize(PipelineAttributeType type)
{
  switch (type)
  {
  	case PipelineAttributeType::Float: { return 4; }
  	case PipelineAttributeType::Float2: { return 4 * 2; }
  	case PipelineAttributeType::Float3: { return 4 * 3; }
  	case PipelineAttributeType::Float4: { return 4 * 4; }
  	case PipelineAttributeType::Int: { return 4; }
  	case PipelineAttributeType::Int2: { return 4 * 2; }
  	case PipelineAttributeType::Int3: { return 4 * 3; }
  	case PipelineAttributeType::Int4: { return 4 * 4; }
  	case PipelineAttributeType::Char: { return 1; }
  	case PipelineAttributeType::Char2: { return 2; }
  	case PipelineAttributeType::Char3: { return 3; }
  	case PipelineAttributeType::Char4: { return 4; }
  	case PipelineAttributeType::UInt: { return 4; }
  	case PipelineAttributeType::UInt2: { return 4 * 2; }
  	case PipelineAttributeType::UInt3: { return 4 * 3; }
  	case PipelineAttributeType::UInt4: { return 4 * 4; }
  	case PipelineAttributeType::UChar: { return 1; }
  	case PipelineAttributeType::UChar2: { return 2; }
  	case PipelineAttributeType::UChar3: { return 3; }
  	case PipelineAttributeType::UChar4: { return 4; }
  }
}

struct PipelineAttribute
{
  String Name;
  PipelineAttributeType Type;
  Size Offset;
  Size Size;
  bool Normalized;
  
  PipelineAttribute() = default;
  
  PipelineAttribute(PipelineAttributeType type, const String& name, bool normalized = false)
  	: Name(name), Type(type), Size(PipelineAttributeTypeSize(type)), Offset(0), Normalized(normalized) {}
};

// In metal it's possible to use layouts with multiple vertex buffers. I don't
// think that we'll run into a case where the usage is super important. We'll
// skip it right now.
struct PipelineLayout
{
  DynamicArray<PipelineAttribute> Attributes;
  Size Stride;
  
  PipelineLayout() = default;
  
  PipelineLayout(InitializerList<PipelineAttribute> attributes)
  	: Attributes(attributes) { CalcOffsetsAndStride(); }
  
  DynamicArray<PipelineAttribute>::Iterator begin() noexcept { return Attributes.begin(); }
  DynamicArray<PipelineAttribute>::Iterator end() noexcept { return Attributes.end(); }
  DynamicArray<PipelineAttribute>::ConstIterator begin() const noexcept { return Attributes.begin(); }
  DynamicArray<PipelineAttribute>::ConstIterator end() const noexcept { return Attributes.end(); }
  
private:
  void CalcOffsetsAndStride()
  {
    Size offset = 0;
    
    for (auto& attr : Attributes)
    {
      attr.Offset = offset;
      offset += attr.Size;
    }
    
    Stride = offset;
  }
};

// ----- PipelineState -----------------

struct PipelineStateDesc
{
  Ref<Shader> Shader;
  PipelineLayout Layout;
};

class PipelineState
{
public:
  virtual ~PipelineState() = default;
};

} // namespace Neptune
