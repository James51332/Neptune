#pragma once

namespace Neptune
{

// ----- Transform ------------

struct TransformComponent
{
  Float3 Position;
  Float3 Rotation;
  Float3 Scale;
  
  TransformComponent()
  	: Position(0.0f), Rotation(0.0f), Scale(1.0f) {}
};

}
