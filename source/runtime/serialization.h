/**
* @file		serialization.h
* @brief	Provides a generic framework for binary serialization of trivially copyable types.
* 
*			This header defines:
*			- A templated `AutoSerializable` class that allows objects to integrate serialization.
*			- An `ISerializable` interface for cases where custom serialization logic is required.
*			- Utility functions to handle raw binary serialization and deserialization.
* 
* @warning	The serialization system relies on raw memory copying, which can introduce security risks
*			such as buffer overflows and arbitrary code execution. While basic bounds checking is
*			implemented, it is **the user's responsibility** to always report correct `bufferSize` values.
*			**Do NOT inherit `AutoSerializable` in non-trivially copyable types**, as this may lead to
*			undefined behavior.
* 
* @example	This framework is optimized for tightly packed data structures to avoid padding issues
*			and improve efficiency. In C++, tightly packed data structures can be defined using
*			`#pragma pack` directives:
*
*			@code
*			#pragma pack(push, 1)
*			struct YourStruct {
*			    int yourData;
*			    ...
*			};
*			#pragma pack(pop)
*			@endcode
*
* @author	Konstantinos Lampros
* @version	1.0.1
* @date		2025-03-13
*/

#pragma once

#include "public/defines.h"
#include <type_traits>

namespace ryn::serialization {
	/**
	* @class	ISerializable
	* @brief	Interface for implementing custom serialization logic.
	*	
	*			Classes inheriting from `ISerializable` must implement the `serialize()` and
	*			`deserialize()` methods, providing a unified interface for binary serialization.
	*			This allows objects to define their own serialization logic instead of relying on
	*			raw memory copying.
	* 
	* @warning	This interface does not enforce a specific serialization format.
	*			Implementations must ensure compatibility between serialization and deserialization.
	*/
	class ISerializable {
	public:
		virtual ~ISerializable() = default;

		/**
		* @brief				Serializes the object into a raw byte buffer.
		*
		* @param buffer			Pointer to the destination buffer.
		* @param bufferSize		Total size of the destination buffer in bytes.
		* @param bufferOffset	Offset within the destination buffer where serialization should begin.
		* @return				Number of bytes written to the destination buffer.
		*/
		virtual size_t serialize(uint8_t* buffer, size_t bufferSize, size_t bufferOffset) const = 0;

		/**
		* @brief				Deserializes the object from a raw byte buffer.
		*
		* @param buffer			Pointer to the source buffer.
		* @param bufferSize		Total size of the source buffer in bytes.
		* @param bufferOffset	Offset within the source buffer from where deserialization should begin.
		* @return				Number of bytes read from the source buffer.
		*/
		virtual size_t deserialize(const uint8_t* buffer, size_t bufferSize, size_t bufferOffset) = 0;
	};

	/**
	* @class		AutoSerializable<T>
	* @brief		Base class providing implicit binary serialization.
	* 
	*				Classes inheriting from `AutoSerializable` gain automatic serialization
	*				through `serialize()` and `deserialize()`, leveraging raw memory copying.
	*				This mechanism is only suitable for **trivially copyable types**.

	* @warning		This class **must not** be used with non-trivially copyable types, as doing so
	*				risks **undefined behavior** or, in extreme cases, **arbitrary code execution**.
	*/
	template <typename Derived>
	class AutoSerializable : public ISerializable {
	public:
		/**
		* @brief				Serializes the object into a raw byte buffer.
		*
		* @warning              Ensure that the correct `bufferSize` is reported to avoid overflows.
		* 
		* @param buffer			Pointer to the destination buffer.
		* @param bufferSize		Total size of the destination buffer in bytes.
		* @param bufferOffset	Offset within the destination buffer where serialization should begin.
		* @return				Number of bytes written to the destination buffer.
		*/
		size_t serialize(uint8_t* buffer, size_t bufferSize, size_t bufferOffset) const {
			// Offset by the size of ISerializable to exclude vtable pointer
			return serializeToBuffer(buffer, bufferSize, bufferOffset, 
				*static_cast<const Derived*>(this), sizeof(ISerializable));
		}

		/**
		* @brief				Deserializes the object from a raw byte buffer.
		*
		* @warning				Ensure that the correct `bufferSize` is reported to avoid overflows.
		*
		* @param buffer			Pointer to the source buffer.
		* @param bufferSize		Total size of the source buffer in bytes.
		* @param bufferOffset	Offset within the source buffer from where deserialization should begin.
		* @return				Number of bytes read from the source buffer.
		*/
		size_t deserialize(const uint8_t* buffer, size_t bufferSize, size_t bufferOffset) {
			// Offset by the size of ISerializable to exclude vtable pointer
			return deserializeFromBuffer(buffer, bufferSize, bufferOffset, 
				*static_cast<Derived*>(this), sizeof(ISerializable));
		}
	};

	/**
	* @brief				Serializes a trivially copyable type into a raw byte buffer.
	*
	*						This function copies the memory of a trivially copyable object into
	*						a raw byte buffer, starting at a specified offset. It can also handle
	*						objects derived from `AutoSerializable` types by bypassing
	*						the trivially copyable check.
	*
	* @warning				Ensure `bufferSize` is sufficient to accommodate the serialized data.
	*						Passing an insufficient buffer will result in no data being written.
	*
	* @error				Returns `0` if the buffer is too small or `bufferOffset` is out of bounds.
	* 
	* @param buffer			Pointer to the destination buffer.
	* @param bufferSize		Total size of the destination buffer in bytes.
	* @param bufferOffset	Offset within the destination buffer where serialization should begin.
	* @param value			The object to serialize, passed by reference.
	* @param valuePtrOffset	(Optional) Offset within the object's memory to begin copying (defaults to 0).
	* @return				Number of bytes written to the destination buffer, or `0` on failure.
	*/
	template <typename T>
	size_t serializeToBuffer(uint8_t* buffer, size_t bufferSize, size_t bufferOffset, const T& value, size_t valuePtrOffset = 0) {
		// Assert that type T is trivially copyable with the exception of all AutoSerializable<T> objects
		static_assert(std::is_trivially_copyable<T>::value || std::is_base_of<AutoSerializable<T>, T>::value, 
			"Type must either be trivially copyable, inherited from AutoSerializable or feature a explicit specialization of 'serializeToBuffer'!");

		// Ensure buffer can hold the serialized data
		size_t byteCount = sizeof(T) - valuePtrOffset;
		if (bufferOffset + byteCount > bufferSize)
			return 0;

		// Copy the data starting from the specified offset within value
		memcpy(buffer + bufferOffset, reinterpret_cast<const uint8_t*>(&value) + valuePtrOffset, byteCount);
		return byteCount;
	}

	/**
	* @brief				Deserializes a trivially copyable type from a raw byte buffer.
	*
	*						This function reconstructs a trivially copyable object from a raw
	*						byte buffer, starting at a specified offset. It also handles objects
	*						derived from `AutoSerializable` types by bypassing the trivially copyable check.
	*
	* @warning				Ensure `bufferSize` is sufficient to accommodate the deserialized data.
	*						Passing an insufficiently small buffer will result in no data being read.
	* 
	* @error				Returns `0` if the buffer is too small or `bufferOffset` is out of bounds.
	* 
	* @param buffer			Pointer to the source buffer.
	* @param bufferSize		Total size of the source buffer in bytes.
	* @param bufferOffset	Offset within the source buffer from where deserialization should begin.
	* @param value			Reference to the object into which we deserialized.
	* @param valuePtrOffset	(Optional) Offset within the object's memory where pasting should begin (defaults to 0).
	* @return				Number of bytes read from the source buffer, or `0` on failure.
	*/
	template <typename T>
	size_t deserializeFromBuffer(const uint8_t* buffer, size_t bufferSize, size_t bufferOffset, T& value, size_t valuePtrOffset = 0) {
		// Assert that type T is trivially copyable with the exception of all AutoSerializable<T> objects
		static_assert(std::is_trivially_copyable<T>::value || std::is_base_of<AutoSerializable<T>, T>::value, 
			"Type must either be trivially copyable, inherited from AutoSerializable or feature a explicit specialization of 'deserializeFromBuffer'!");

		// Ensure buffer has enough data for deserialization
		size_t byteCount = sizeof(T) - valuePtrOffset;
		if (bufferOffset + byteCount > bufferSize)
			return 0;

		// Copy the data into value starting from the specified offset
		memcpy(reinterpret_cast<uint8_t*>(&value) + valuePtrOffset, buffer + bufferOffset, byteCount);
		return byteCount;
	}
}