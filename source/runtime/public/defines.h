/**
* @file		defines.h
* @brief	Common macro definitions, platform-specific configurations and utility headers for the engine.
* @author	Konstantinos Lampros
* @version	1.0.0
* @date		2025-01-12
*/

#pragma once

#include <cstdint>

/**
* @brief Define the RYN_API macro for different platforms.
*        This macro is used for dynamic linking and will export or import functions
*        depending on whether the RYN_EXPORT preprocessor directive is defined.
*/
#ifdef RYN_PLATFORM_WIN
	#ifdef RYN_EXPORT
		#define RYN_API __declspec(dllexport) // Export functions when building the DLL.
	#else
		#define RYN_API __declspec(dllimport) // Import functions when using the DLL.
	#endif // RYN_EXPORT
#else
	/**
	* @brief Error message when the platform is unsupported.
	* @warning Only Windows platform is currently supported.
	*/
	#error "RYN_API is only supported on Windows. Please define RYN_PLATFORM_WIN to use the API. Support for other platforms has not been implemented yet."
#endif // RYN_PLATFORM_WIN
