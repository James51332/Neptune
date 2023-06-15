#include "neptunepch.h"
#include "Profile.h"

#include "Application.h"

namespace Neptune
{

// ----- Profiler -----------

// Stack allocated tool to track performance of function.

Profiler::Profiler()
{
}

void Profiler::Start()
{
  // We don't care if the profiler is already running. We'll just update the start time.
  // if (!m_Stopped) Stop();
  
  m_Time = Application::PollTime();
  m_Running = true;
}

void Profiler::Stop()
{
  if (!m_Running) return;
  
  m_Time = Application::PollTime() - m_Time;
  m_Running = false;
}

Float32 Profiler::GetTime()
{
  if (m_Running) Stop();
  return m_Time;
}

// ----- ScopedProfiler -----------

ScopedProfiler::ScopedProfiler(Size& ID)
	: m_ID(ScopedProfilerRegistry::Get().NextID())
{
  Start();
  ID = m_ID;
}

ScopedProfiler::~ScopedProfiler()
{
  Stop();
  ScopedProfilerRegistry::Get().Submit(*this);
}

// ----- ScopedProfilerRegistry -----------

ScopedProfilerRegistry::ScopedProfilerRegistry(Size profilerFrameLimit)
	: m_CurrentID(0), m_MaxID(profilerFrameLimit - 1), m_ProfilerTimes(profilerFrameLimit)
{
}

Float32 ScopedProfilerRegistry::GetTime(Size ID)
{
  ScopedProfilerRegistry& reg = Get();
  NEPTUNE_ASSERT(ID <= reg.m_MaxID, "Invalid Profiler ID!");
  return reg.m_ProfilerTimes[ID];
}

void ScopedProfilerRegistry::Reset()
{
  m_CurrentID = 0; // No point in costly removing of times
}

void ScopedProfilerRegistry::Submit(ScopedProfiler& profiler)
{
  m_ProfilerTimes[profiler.GetID()] = profiler.GetTime();
}

Size ScopedProfilerRegistry::NextID()
{
  if (m_CurrentID == m_MaxID) Reset(); // Loop the buffer back if we overflow
  
  return m_CurrentID++;
}

ScopedProfilerRegistry& ScopedProfilerRegistry::Get()
{
  static ScopedProfilerRegistry s_Registry;
  return s_Registry;
}


} // namespace Neptune
