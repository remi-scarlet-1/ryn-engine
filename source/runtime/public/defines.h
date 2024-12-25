#pragma once

#include <cstdint>

#ifdef RYN_PLATFORM_WIN
	#ifdef RYN_EXPORT
		#define RYN_API __declspec(dllexport)
	#else
		#define RYN_API __declspec(dllimport)
	#endif // RYN_EXPORT
#else
	#error "Unsupported platform! You must define a valid RYN_PLATFORM_* macro."
#endif // RYN_PLATFORM_WIN
