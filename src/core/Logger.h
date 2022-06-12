#pragma once

// #include "utils/SmartPointer.h" (spdlog uses shared_ptr)

#include "core/Intrinsics.h"
#include "utils/String.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

namespace Neptune
{

class Logger
{
public:
  static void Initialize();
  
	static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
  static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }
  
private:
  static std::shared_ptr<spdlog::logger> m_CoreLogger;
  static std::shared_ptr<spdlog::logger> m_ClientLogger;
};

inline std::ostream& operator<<(std::ostream& os, const String& s) {
  return (os << s.Raw());
}

#ifdef NEPTUNE_LOG

#define NEPTUNE_CORE_TRACE(...) ::Neptune::Logger::GetCoreLogger()->trace(__VA_ARGS__);
#define NEPTUNE_CORE_INFO(...)  ::Neptune::Logger::GetCoreLogger()->info(__VA_ARGS__);
#define NEPTUNE_CORE_WARN(...)  ::Neptune::Logger::GetCoreLogger()->warn(__VA_ARGS__);
#define NEPTUNE_CORE_ERROR(...) ::Neptune::Logger::GetCoreLogger()->error(__VA_ARGS__);
#define NEPTUNE_CORE_FATAL(...) ::Neptune::Logger::GetCoreLogger()->fatal(__VA_ARGS__);

#define NEPTUNE_TRACE(...) ::Neptune::Logger::GetClientLogger()->trace(__VA_ARGS__);
#define NEPTUNE_INFO(...)  ::Neptune::Logger::GetClientLogger()->info(__VA_ARGS__);
#define NEPTUNE_WARN(...)  ::Neptune::Logger::GetClientLogger()->warn(__VA_ARGS__);
#define NEPTUNE_ERROR(...) ::Neptune::Logger::GetClientLogger()->error(__VA_ARGS__);
#define NEPTUNE_FATAL(...) ::Neptune::Logger::GetClientLogger()->fatal(__VA_ARGS__);

#else

#define NEPTUNE_CORE_TRACE(...)
#define NEPTUNE_CORE_INFO(...)
#define NEPTUNE_CORE_WARN(...)
#define NEPTUNE_CORE_ERROR(...)
#define NEPTUNE_CORE_FATAL(...)

#define NEPTUNE_TRACE(...)
#define NEPTUNE_INFO(...)
#define NEPTUNE_WARN(...)
#define NEPTUNE_ERROR(...)
#define NEPTUNE_FATAL(...)

#endif

} // namespace Neptune
