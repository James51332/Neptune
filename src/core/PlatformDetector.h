#pragma once

#if defined(_WIN32)

#if defined(_WIN64)
	#define NEPTUNE_WINDOWS
#else
	#error "x86 builds aren't supported!"
#endif

#elif defined(__APPLE__)

#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
	#error "iOS isn't supported!"
#elif TARGET_OS_MACCATALYST == 1
	#error "Catalyst isn't supported!"
#elif TARGET_OS_IPHONE == 1
	#error "iOS isn't supported!"
#elif TARGET_OS_MAC == 1
	#define NEPTUNE_MACOS
#else
	#error "Unknown Apple platform"
#endif

#elif defined(__linux__)

#define NEPTUNE_LINUX

#else

#error "Unknown compiler"

#endif
