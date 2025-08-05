/**
 * @file		defines.h
 * @brief		Common macros, platform configurations, and utility includes.
 *
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

#include <cstdint>

// Define RYN_API macro used for dynamic library export and import.
#ifdef RYN_PLATFORM_WIN
	#ifdef RYN_RUNTIME
		#define RYN_API __declspec(dllexport)
	#else
		#define RYN_API __declspec(dllimport)
	#endif
#else
	#error "RYN_API (shared library export/import) is currently only supported on Windows. Please define RYN_PLATFORM_WIN or implement support for other platforms."
#endif
