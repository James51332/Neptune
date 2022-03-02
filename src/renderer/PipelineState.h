#pragma once

#include "Shader.h"

namespace Neptune
{

// ----- PipelineState -----------------

// TODO: We'll need to add layouts for buffers
struct PipelineStateDesc
{
  Ref<Shader> Shader;
};

class PipelineState
{
public:
  virtual ~PipelineState() = default;
};

} // namespace Neptune
