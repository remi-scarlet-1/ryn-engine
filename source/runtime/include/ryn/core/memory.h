#pragma once

#include "ryn/defines.h"

namespace ryn {
	template<typename T>
	struct DefaultDeleter {
		void operator()(T* ptr) const noexcept {
			delete ptr;
		}
	};

	template<typename T, typename Deleter = DefaultDeleter<T>>
	class RYN_API UniquePtr {
	public:
		explicit UniquePtr(T* ptr = nullptr, Deleter deleter = Deleter{}) noexcept
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
		T* ptr;
		Deleter deleter;
	};

	template<typename T, typename Deleter = DefaultDeleter<T>>
	class RYN_API SharedPtr {
	public:
		explicit SharedPtr(T* ptr, Deleter deleter = Deleter{}) noexcept
			: control(ptr ? new ControlBlock(ptr, deleter) : nullptr) {}

		~SharedPtr() { release(); }

		SharedPtr(const SharedPtr& other) noexcept {
			control = other.control;
			if (control) ++control->refCount;
		}

		SharedPtr& operator=(const SharedPtr& other) noexcept {
			if (this != &other) {
				release();
				control = other.control;
				if (control) ++control->refCount;
			}
			return *this;
		}

		SharedPtr(SharedPtr&& other) noexcept {
			control = other.control;
			other.control = nullptr;
		}

		SharedPtr& operator=(SharedPtr&& other) noexcept {
			if (this != &other) {
				release();
				control = other.control;
				other.control = nullptr;
			}
			return *this;
		}

		T& operator*() const noexcept { return *control->ptr; }
		T* operator->() const noexcept { return control->ptr; }
		[[nodiscard]] T* get() const noexcept { return control ? control->ptr : nullptr; }
		[[nodiscard]] explicit operator bool() const noexcept { return control && control->ptr; }

		size_t getRefCount() const noexcept { return control ? control->refCount : 0; }

	private:
		struct ControlBlock {
			T* ptr;
			Deleter deleter;
			size_t refCount;

			ControlBlock(T* ptr, Deleter deleter) : ptr(ptr), deleter(deleter), refCount(1) {}
		};

		ControlBlock* control;

		void release() noexcept {
			if (control) {
				if (--control->refCount == 0) {
					control->deleter(control->ptr);
					delete control;
				}
				control = nullptr;
			}
		}
	};
}