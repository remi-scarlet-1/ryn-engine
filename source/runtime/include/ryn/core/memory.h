#pragma once

#include "ryn/defines.h"

namespace ryn {
	template<typename T>
	class RYN_API UniquePtr {
	public:
		using DeleterFunction = void(*)(T*);

		// Explicit constructor and normal destructor
		explicit UniquePtr(T* ptr = nullptr, DeleterFunction deleter = &defaultDeleter) noexcept 
			: ptr(ptr), deleter(deleter) {}
		~UniquePtr() { reset(); }

		// Delete copy constructor and assignment 
		UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

		// Move constructor and assignment
		UniquePtr(UniquePtr&& other) noexcept 
			: ptr(other.ptr), deleter(other.deleter) { other.ptr = nullptr; }

		UniquePtr& operator=(UniquePtr&& other) noexcept {
			if (this != &other) {
				reset();
				ptr = other.ptr;
				deleter = other.deleter;
				other.ptr = nullptr;
			}
			return *this;
		}

		// Implement utility functions and operator overrides
		T& operator*() const noexcept { return *ptr; }
		T* operator->() const noexcept { return ptr; }
		[[nodiscard]] constexpr T* get() const noexcept { return ptr; }
		[[nodiscard]] constexpr explicit operator bool() const noexcept { return ptr != nullptr; }

		// Resets the internal pointer to a new value making sure to free the resource in the process
		void reset(T* newPtr = nullptr) noexcept {
			if (ptr != newPtr) {
				if (ptr)
					deleter(ptr);
				ptr = newPtr;
			}
		}

		// Releases the underlying pointer without freeing its resources
		[[nodiscard]] T* release() noexcept {
			T* oldPtr = ptr;
			ptr = nullptr;
			return oldPtr;
		}

	private:
		static void defaultDeleter(T* ptr) { delete ptr; }

		T* ptr;
		DeleterFunction deleter;
	};
}