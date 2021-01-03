/// \file
/// \brief Internal header file that describes the utility functions for
/// transforming data.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <climits>

namespace std::io::Utilities
{

/// \brief A concept specifying ISO 60559 floating point type.
template <typename T>
concept iso60559 = floating_point<T> && (numeric_limits<T>::is_iec559 == true);

/// \brief A concept specifying ISO 60559 binary32 type with no padding.
template <typename T>
concept iso60559_binary32 = iso60559<T> &&
	(numeric_limits<T>::radix == 2) &&
	(numeric_limits<T>::digits = 24) &&
	(numeric_limits<T>::min_exponent == -125) &&
	(numeric_limits<T>::max_exponent == 128) &&
	(sizeof(T) * CHAR_BIT == 32);

/// \brief A concept specifying ISO 60559 binary64 type with no padding.
template <typename T>
concept iso60559_binary64 = iso60559<T> &&
	(numeric_limits<T>::radix == 2) &&
	(numeric_limits<T>::digits = 53) &&
	(numeric_limits<T>::min_exponent == -1021) &&
	(numeric_limits<T>::max_exponent == 1024) &&
	(sizeof(T) * CHAR_BIT == 64);

/// \brief A concept specifying a sane ISO 60559 type.
template <typename T>
concept iso60559_sane = iso60559_binary32<T> || iso60559_binary64<T>;

/// \brief Converts given bytes in the specified format to the integral object
/// in the native format.
/// \param[in] in_buffer Buffer with bytes to transform.
/// \param[in] f Format of the bytes.
/// \param[out] object Object to write to.
/*constexpr*/ void ToNative(span<const byte> in_buffer, format f,
	integral auto& object);

/// \brief Converts given bytes in the specified format to the floating point
/// object in the native format.
/// \param[in] in_buffer Buffer with bytes to transform.
/// \param[in] f Format of the bytes.
/// \param[out] object Object to write to.
/// \throw std::runtime_error If floating point format or endianness is not
/// supported.
/*constexpr*/ void ToNative(span<const byte> in_buffer, format f,
	floating_point auto& object);

/// \brief Converts given bytes in the specified format to the ISO 60559 object
/// in the native format.
/// \param[in] in_buffer Buffer with bytes to transform.
/// \param[in] f Format of the bytes.
/// \param[out] object Object to write to.
/// \throw std::runtime_error If given endianness is not native and native
/// endianness is not supported.
/*constexpr*/ void ToISO60559(span<const byte> in_buffer, format f,
	iso60559_sane auto& object);

/// \brief Converts given integral object in the native format to the bytes in
/// the specified format.
/// \tparam T Type of the variable to read from.
/// \param[in] object Object to convert.
/// \param[in] f Format to convert to.
/// \param[out] out_buffer Buffer to write to.
template <typename T>
requires integral<T> || is_enum_v<T>
/*constexpr*/ void FromNative(T object, format f, span<byte> out_buffer);

/// \brief Converts given floating point object in the native format to the
/// bytes in the specified format.
/// \param[in] object Object to convert.
/// \param[in] f Format to convert to.
/// \param[out] out_buffer Buffer to write to.
/// \throw std::runtime_error If floating point format or endianness is not
/// supported.
/*constexpr*/ void FromNative(floating_point auto object, format f,
	span<byte> out_buffer);

/// \brief Converts given ISO 60559 object in the native format to the bytes in
/// the specified format.
/// \param[in] object Object to convert.
/// \param[in] f Format to convert to.
/// \param[out] out_buffer Buffer to write to.
/// \throw std::runtime_error If given endianness is not native and native
/// endianness is not supported.
/*constexpr*/ void FromISO60559(iso60559_sane auto object, format f,
	span<byte> out_buffer);

/// \brief Swaps bytes of the given buffer if the given endianness requires it.
/// \param[in] endianness Endianness to check.
/// \param[in,out] buffer Buffer with bytes to swap.
/// \throw std::runtime_error If given endianness is not native and native
/// endianness is not supported.
constexpr void SwapBytes(endian endianness, span<byte> buffer);

}

#include "format_utilities.hpp"
