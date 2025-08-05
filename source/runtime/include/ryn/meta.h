/**
 * @file        meta.h
 * @brief       Compile-time type utilities and identifiers for strong typing support.
 * @details     Provides TypeID for runtime type identity and forwarding utilities for perfect forwarding.
 *              All constructs are lightweight and STL-independent except for standard hashing.
 *              Intended for internal engine use; not part of public API surface.
 *
 * @author      Konstantinos Lampros
 * @version     1.0.0
 * @date        2025-08-05
 */

#pragma once

#include <functional>
#include <type_traits>

/**
 * @namespace	ryn::meta
 * @brief       Contains compile-time utilities and constructs for meta programming.
 * @details     Designed to support strong typing, type erasure, and lightweight reflection without STL containers.
 *              All utilities are constexpr where possible and have minimal runtime overhead.
 */
namespace ryn::meta {
	/**
	 * @struct		TypeID
	 * @brief       Represents a unique, compile-time identifier for a type T.
	 * @details     TypeIDs are stable during a single application execution. Useful for type-erased storage
	 *				and runtime type comparison. Comparison is based on address identity of an internal static marker.
	 * @attention	Not intended for usage on its own. To get the TypeID of a type, use getTypeID.
	 */
	struct TypeID {
		const void* id;
		bool operator==(const TypeID& other) const { return id == other.id; }
		bool operator!=(const TypeID& other) const { return id != other.id; }
	};

	/**
	 * @brief       Retrieves a unique TypeID for a given type T.
	 * @tparam      T The type for which to generate a TypeID.
	 * @return      A TypeID that uniquely identifies type T at runtime.
	 * @note        TypeIDs are guaranteed unique per type within the same binary.
	 */
	template<typename T>
	constexpr TypeID getTypeID() {
		static const char uniqueID = 0;
		return TypeID{ .id = static_cast<const void*>(&uniqueID) };
	}

	/**
	 * @brief       Perfect forwarding utility (l-value overload).
	 * @details     Forwards an l-value reference while preserving its type qualifiers.
	 * @tparam      T The target type to forward as.
	 * @param       t The value to forward.
	 * @return      A forwarded reference of type T&&.
	 */
	template<typename T>
	constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	/**
	 * @brief       Perfect forwarding utility (r-value overload).
	 * @details     Forwards an r-value reference while preserving its type qualifiers.
	 *              Validates that T is not an l-value reference to prevent misuse.
	 * @tparam      T The target type to forward as.
	 * @param       t The value to forward.
	 * @return      A forwarded reference of type T&&.
	 */
	template<typename T>
	constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
		static_assert(!std::is_lvalue_reference_v<T>, "bad forward of r value");
		return static_cast<T&&>(t);
	}
}

namespace std {
	/**
	 * @brief       Specialization of std::hash for ryn::meta::TypeID.
	 * @details     Enables use of ryn::meta::TypeID in hash-based containers such as unordered_map or unordered_set.
	 */
	template<>
	struct hash<ryn::meta::TypeID> {
		std::size_t operator()(const ryn::meta::TypeID& t) const noexcept {
			return std::hash<const void*>{}(t.id);
		}
	};
}