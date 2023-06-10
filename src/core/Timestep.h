#pragma once

namespace Neptune
{

class Timestep
{
public:
  Timestep(Float32 ts) : m_DeltaTime(ts) {}
  Timestep() : m_DeltaTime(0) {}
  
  operator Float32() const { return static_cast<Float32>(m_DeltaTime); }
  
private:
  Float32 m_DeltaTime;
};

} // namespace Neptune
