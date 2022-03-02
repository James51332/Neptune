#pragma once

namespace Neptune
{

// ----- Shader -----------------

// A shader is a program run by the GPU. There are two
// primary types of shaders in the render pipeline:
// vertex and fragment shaders which handle geometry and
// pixels respectively. These are created (compiled) and
// bundled into pipeline state objects and don't really need
// an API beyond creation via a RenderDevice.

struct ShaderDesc
{
  const char* vertexSrc;
  const char* fragmentSrc;
};

class Shader
{
public:
  virtual ~Shader() = default;
};

} // namespace Neptune
