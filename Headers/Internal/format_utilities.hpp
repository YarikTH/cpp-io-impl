/// \file
/// \brief Internal header file that contains implementation of the utility
/// functions for transforming data.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <cstddef>
#include <cstring>
#include <algorithm>

namespace std::io::Utilities
{

/*constexpr*/ void ToNative(span<const byte> in_buffer, format f,
	integral auto& object)
{
	if constexpr (sizeof(object) == 1)
	{
		object = to_integer<decltype(object)>(in_buffer[0]);
		return;
	}
	auto endianness = f.get_endianness();
	if (endianness == endian::native)
	{
		// TODO: bit_cast.
		memcpy(&object, ranges::data(in_buffer), sizeof(object));
		return;
	}
	make_unsigned_t<remove_reference_t<decltype(object)>> out_buffer = 0;
	if (endianness == endian::little)
	{
		auto it = ranges::begin(in_buffer);
		for (size_t i = 0; i < sizeof(object); ++i)
		{
			out_buffer += to_integer<unsigned char>(*it) << (CHAR_BIT * i);
			++it;
		}
	}
	else if (endianness == endian::big)
	{
		auto it = ranges::rbegin(in_buffer);
		for (size_t i = 0; i < sizeof(object); ++i)
		{
			out_buffer += to_integer<unsigned char>(*it) << (CHAR_BIT * i);
			++it;
		}
	}
	// TODO: bit_cast.
	memcpy(&object, &out_buffer, sizeof(object));
}

/*constexpr*/ void ToNative(span<const byte> in_buffer, format f,
	floating_point auto& object)
{
	auto float_format = f.get_floating_point_format();
	if (float_format == floating_point_format::native)
	{
		// TODO: bit_cast.
		memcpy(&object, ranges::data(in_buffer), sizeof(object));
		return;
	}
	if constexpr (iso60559_sane<decltype(object)>)
	{
		ToISO60559(in_buffer, f, object);
		return;
	}
	else
	{
		// IEC-to-native conversion here
		throw runtime_error{"Floating point format is unsupported."};
	}
}

/*constexpr*/ void ToISO60559(span<const byte> in_buffer, format f,
	iso60559_sane auto& object)
{
	array<byte, sizeof(object)> buffer;
	ranges::copy(ranges::begin(in_buffer), ranges::begin(in_buffer) +
		sizeof(object), ranges::begin(buffer));
	SwapBytes(f.get_endianness(), buffer);
	// TODO: bit_cast.
	memcpy(&object, ranges::data(buffer), sizeof(object));
}

template <typename T>
requires integral<T> || is_enum_v<T>
/*constexpr*/ void FromNative(T object, format f, span<byte> out_buffer)
{
	if constexpr (sizeof(object) == 1)
	{
		// TODO: bit_cast.
		memcpy(ranges::data(out_buffer), &object, sizeof(object));
		return;
	}
	auto endianness = f.get_endianness();
	if (endianness == endian::native)
	{
		// TODO: bit_cast.
		memcpy(ranges::data(out_buffer), &object, sizeof(object));
		return;
	}
	auto unsigned_value =
		static_cast<make_unsigned_t<decltype(object)>>(object);
	if (endianness == endian::little)
	{
		auto it = ranges::begin(out_buffer);
		for (size_t i = 0; i < sizeof(object); ++i)
		{
			*it = byte{static_cast<unsigned char>(
				unsigned_value >> (CHAR_BIT * i))};
			++it;
		}
	}
	else if (endianness == endian::big)
	{
		auto it = ranges::rbegin(out_buffer);
		for (size_t i = 0; i < sizeof(object); ++i)
		{
			*it = byte{static_cast<unsigned char>(
				unsigned_value >> (CHAR_BIT * i))};
			++it;
		}
	}
}

/*constexpr*/ void FromNative(floating_point auto object, format f,
	span<byte> out_buffer)
{
	auto float_format = f.get_floating_point_format();
	if (float_format == floating_point_format::native)
	{
		// TODO: bit_cast.
		memcpy(ranges::data(out_buffer), &object, sizeof(object));
		return;
	}
	if constexpr (iso60559_sane<decltype(object)>)
	{
		FromISO60559(object, f, out_buffer);
		return;
	}
	else
	{
		// Native-to-IEC conversion here
		throw runtime_error{"Floating point format is unsupported."};
	}
}

/*constexpr*/ void FromISO60559(iso60559_sane auto object, format f,
	span<byte> out_buffer)
{
	// TODO: bit_cast.
	memcpy(ranges::data(out_buffer), &object, sizeof(object));
	SwapBytes(f.get_endianness(), out_buffer);
}

constexpr void SwapBytes(endian endianness, span<byte> buffer)
{
	if (endianness == endian::native)
	{
		return;
	}
	if (((endian::native == endian::little) && (endianness == endian::big)) ||
		((endian::native == endian::big) && (endianness == endian::little)))
	{
		ranges::reverse(buffer);
		return;
	}
	if ((endian::native != endian::little) && (endian::native != endian::big))
	{
		throw runtime_error{
			"Native endianness is not supported. Are you on PDP?"};
	}
}

}
