/**
 * @file		defines.h
 * @brief		Common macro definitions, platform-specific configurations and utility headers for the engine.
 *
 * @author		Konstantinos Lampros
 * @version		1.0.4
 * @date		2025-07-22
 */

#pragma once

#include <cstdint>

/**
 * @brief	Define a RYN_API macro which is used for dynamic linking and will import or export
 *			functions depending on whether the RYN_API_EXPORT preprocessor directive is defined.
 */
#ifdef RYN_PLATFORM_WIN
	#ifdef RYN_RUNTIME
		#define RYN_API __declspec(dllexport)
	#else
		#define RYN_API __declspec(dllimport)
	#endif
#else
	#error "RYN_API (shared library symbol export/import) is currently only supported on Windows. Please define RYN_PLATFORM_WIN or implement support for other platforms."
#endif
