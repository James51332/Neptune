#include "neptunepch.h"
#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace Neptune
{

std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;
std::shared_ptr<spdlog::logger> Logger::m_ClientLogger;

void Logger::Initialize()
{
  spdlog::set_pattern("%^[%T] %n: %v%$");
  spdlog::set_level(spdlog::level::trace);
  
  m_CoreLogger = spdlog::stdout_color_mt("CORE");
  m_ClientLogger = spdlog::stdout_color_mt("APP");
}


} // namespace Neptune
