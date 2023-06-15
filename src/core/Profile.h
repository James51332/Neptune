#pragma once

#include <string_view>

namespace Neptune
{

// ----- Profiler -----------

// Small object to track time.
class Profiler
{
public:
  Profiler();
  
  void Start();
  void Stop();
  
  // Get time of profiler in seconds
  Float32 GetTime();
  
  bool GetRunning() const { return !m_Running; }
  
private:
  // When running, holds the start time
  // Once stopped, holds the delta time (seconds).
  Float32 m_Time = 0;
  bool m_Running = false;
};

// ----- ScopedProfiler -----------

// Small stack allocated object to track time. Accessed via ProfilerRegistry
class ScopedProfiler : public Profiler
{
public:
  ScopedProfiler(Size& ID); // Pass a reference to where the profiler ID is stored
  
  ~ScopedProfiler();
  
  // Delete all copy and move operators (never should be used)
  ScopedProfiler(const ScopedProfiler& other) = delete;
  
  Size GetID() const { return m_ID; }
  
private:
  Size m_ID = 0;
};

// ----- ScopedProfilerRegistry -----------

// The profile registry is a buffer which stores all scoped profilers automatically.
// For the most part, it accessed via a global instance. It is possible to create an
// instance of this class, but it's pointless.
class ScopedProfilerRegistry
{
  friend class ScopedProfiler;
public:
  // ctor with buffer size. buffer will overflow to beginning if too many are used.
  ScopedProfilerRegistry(Size profilerFrameLimit = 10000);
  
  // Convenience static function to access global instance
  static Float32 GetTime(Size ID);
  
  // Instance API (not really public, unless one created their own registry, which is pointless)
  void Reset();
  
  void Submit(ScopedProfiler& profiler);
  Size NextID();
  
private:
  // This method is only accessible to friends (ScopedProfiler)
  static ScopedProfilerRegistry& Get();
  
private:
  DynamicArray<Float32> m_ProfilerTimes;
  
  Size m_MaxID;
  Size m_CurrentID = 0;
};


} // namespace Neptune
