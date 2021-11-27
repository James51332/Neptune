#pragma once

#include "core/PlatformDetector.h"
#include "core/Logger.h"

#ifdef NEPTUNE_DEBUG

#if defined(NEPTUNE_MACOS) || defined(NEPTUNE_LINUX)
	#include <signal.h>
	#ifdef SIGTRAP
		#define NEPTUNE_DEBUGBREAK() raise(SIGTRAP)
	#else
		#define NEPTUNE_DEBUGBREAK() raise(SIGABRT)
	#endif
#elif defined(NEPTUNE_WINDOWS)
	#define NEPTUNE_DEBUGBREAK() __debugbreak()
#else
	#error "Debugbreak is not supported on this platform!"
#endif

#define NEPTUNE_ASSERT(x, msg) 										 \
{															 										 \
  if (!(x))										 										 \
	{														 										 \
    NEPTUNE_CORE_ERROR("Failed Assert: {}", msg);	 \
    NEPTUNE_DEBUGBREAK();			 										 \
  }														 										 \
}

#else

#define NEPTUNE_DEBUGBREAK()

#define NEPTUNE_ASSERT(x, msg)

#endif
