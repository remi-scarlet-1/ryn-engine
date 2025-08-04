#pragma once

#include <functional>
#include <type_traits>

namespace ryn {
	// TODO: document
	struct TypeID {
		const void* id;
		bool operator==(const TypeID& other) const { return id == other.id; }
		bool operator!=(const TypeID& other) const { return id != other.id; }
	};

	// TODO: document
	template<typename T>
	constexpr TypeID getTypeID() {
		static const char uniqueID = 0;
		return TypeID{ .id = static_cast<const void*>(&uniqueID) };
	}

	// TODO: document
	template<typename T>
	constexpr T&& forward(std::remove_reference_t<T>& t) noexcept {
		return static_cast<T&&>(t);
	}

	// TODO: document
	template<typename T>
	constexpr T&& forward(std::remove_reference_t<T>&& t) noexcept {
		static_assert(!std::is_lvalue_reference_v<T>, "bad forward of r value");
		return static_cast<T&&>(t);
	}
}

// TODO: document
namespace std {
	template<>
	struct hash<ryn::TypeID> {
		std::size_t operator()(const ryn::TypeID& t) const noexcept {
			return std::hash<const void*>{}(t.id);
		}
	};
}