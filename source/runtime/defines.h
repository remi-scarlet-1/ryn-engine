/**
* @file		defines.h
* @brief	Common macro definitions, platform-specific configurations and utility headers for the engine.
* @author	Konstantinos Lampros
* @version	1.0.2
* @date		2025-03-06
*/

#pragma once

#include <cstdint>

/**
* @brief Define the RYN_API macro for different platforms.
*        This macro is used for dynamic linking and will export or import functions
*        depending on whether the RYN_EXPORT preprocessor directive is defined.
*/
#ifdef RYN_PLATFORM_WIN
	#ifdef RYN_ENGINE
		#define RYN_API __declspec(dllexport) // Export functions when building the DLL.
	#else
		#define RYN_API __declspec(dllimport) // Import functions when using the DLL.
	#endif
#else
	#error "RYN_API (shared library symbol export/import) is currently only supported on Windows. Please define RYN_PLATFORM_WIN or implement support for other platforms."
#endif
