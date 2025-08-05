/**
 * @file		namespaces.h
 * @brief		Amalgamation of namespaces.
 * @details		Forward declares all API namespaces for centralized documentation.
 * 
 * @author		Konstantinos Lampros
 * @version		1.0.0
 * @date		2025-08-05
 */

#pragma once

/**
 * @namespace	ryn
 * @brief		Unified engine namespace.
 */
namespace ryn {
	/**
	 * @namespace	ryn::core
	 * @brief		Core components.
	 * @details		Provides essential core components for the runtime. Includes types such as ryn::core::Game, which manages the
	 *				applications lifecycle or ryn::core::Subsystem, which acts as the base class for all engine systems including
	 *				atomic ECS systems.
	 */
	namespace core {}

	/**
	 * @namespace	ryn::entrypoint
	 * @brief		Engine bootstrap logic.
	 * @details		Provides the engine bootstrap logic and main loop in addition to a hidden main implementation for consumers.
	 * @see			entrypoint.h
	 */
	namespace entrypoint {}

	/**
	 * @namespace	ryn::memory
	 * @brief		Memory management utilities.
	 * @details		Provides constructs to aid with manual memory management. For example non-STL smart pointers 
	 *				such as UniquePtr, SharedPtr, and OptionalPtr and more.
	 * 
	 * @attention	Most tools inside this namespace are intended for **internal engine use only**. Usage outside the runtime binary
	 *				or over DLL boundaries may cause ABI breaks or undefined behavior. **Include with care**!
	 */
	namespace memory {}

	/**
	 * @namespace	ryn::meta
	 * @brief       Compile-time utilities and constructs for meta programming.
	 * @details     Provides different compile time utilities in addition to constructs for lightweight reflection without STL containers.
	 * 
	 * @attention	Most tools inside this namespace are intended for **internal engine use only**. Usage outside the runtime binary
	 *				or over DLL boundaries may cause ABI breaks or undefined behavior. **Include with care**!
	 */
	namespace meta {}
}