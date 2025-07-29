/**
 * @file		memory.h
 * @brief		Provides smart pointer types for manual memory management without relying on STL,
 *				designed to maintain ABI stability and avoid exposing standard library types.
 *
 * @author		Konstantinos Lampros
 * @version		1.1.0
 * @date		2025-07-29
 */

#pragma once

#include "ryn/defines.h"

namespace ryn {
	/**
	 * @struct	DefaultDeleter
	 * @brief	Provides a default deleter used by smart pointers to free resources. Simply calls delete
	 *			on a given pointer.
	 * @tparam	T: Type of the object to delete.
	 */
	template<typename T>
	struct DefaultDeleter {
		void operator()(T* ptr) const noexcept {
			delete ptr;
		}
	};

	/**
	 * @class	UniquePtr
	 * @brief	A non-copyable, movable smart pointer managing unique ownership of a dynamically allocated object.
	 *			Provides ownership semantics, forbidding copy operations but allowing moves. Deletes managed object 
	 *			when destroyed or reset.
	 * @tparam	T: Type of the managed object.
	 * @tparam	Deleter: Callable type used to delete the managed object (default: DefaultDeleter<T>).
	 */
	template<typename T, typename Deleter = DefaultDeleter<T>>
	class RYN_API UniquePtr {
	public:
		explicit UniquePtr(T* ptr = nullptr, Deleter deleter = Deleter{}) noexcept
			: ptr(ptr), deleter(deleter) {}

		~UniquePtr() { reset(); }

		UniquePtr(const UniquePtr&) = delete;
		UniquePtr& operator=(const UniquePtr&) = delete;

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

		T& operator*() const noexcept { return *ptr; }
		T* operator->() const noexcept { return ptr; }
		[[nodiscard]] constexpr T* get() const noexcept { return ptr; }
		[[nodiscard]] constexpr explicit operator bool() const noexcept { return ptr != nullptr; }

		/**
		 * @brief	Replaces the managed object with a new pointer, deleting the old one if necessary.
		 * @param	newPtr: Pointer to new object to manage (default nullptr).
		 */
		void reset(T* newPtr = nullptr) noexcept {
			if (ptr != newPtr) {
				if (ptr)
					deleter(ptr);
				ptr = newPtr;
			}
		}

		/**
		 * @brief	Releases ownership of the managed object without deleting it.
		 * @return	The pointer to the managed object before release.
		 */
		[[nodiscard]] T* release() noexcept {
			T* oldPtr = ptr;
			ptr = nullptr;
			return oldPtr;
		}

	private:
		T* ptr;
		Deleter deleter;
	};

	/**
	 * @class	SharedPtr
	 * @brief	A reference-counted smart pointer sharing ownership of a dynamically allocated object.
	 *			When the last SharedPtr owning the object is destroyed or reset, the object is deleted.
	 * @tparam	T: Type of the managed object.
	 * @tparam	Deleter: Callable type used to delete the managed object (default: DefaultDeleter<T>).
	 * @warning	This implementation is **not thread-safe**. Concurrent access or modification will cause raise conditions.
	 */
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

		/**
		 * @brief	Returns the current reference count.
		 * @return	Number of SharedPtr instances sharing ownership of the object.
		 */
		size_t getRefCount() const noexcept { return control ? control->refCount : 0; }

	private:
		struct ControlBlock {
			T* ptr;
			Deleter deleter;
			size_t refCount;

			ControlBlock(T* ptr, Deleter deleter) : ptr(ptr), deleter(deleter), refCount(1) {}
		};

		ControlBlock* control;

		/**
		 * @brief Decrements the reference count and deletes the managed object and control block
		 *        if this was the last owner.
		 */
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