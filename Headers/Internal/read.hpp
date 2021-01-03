/// \file
/// \brief Internal header file that contains implementation of the read
/// customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "read_raw.h"
#include "format_utilities.h"

namespace std::io
{
namespace CustomizationPoints
{

template <typename T, typename I, typename... Args>
requires CustomlyReadableFrom<T, I, Args...>
constexpr void read(T& object, I& i, Args&&... args)
{
	object.read(i, forward<Args>(args)...);
}

constexpr void read(byte& object, input_stream auto& s)
{
	read_raw(object, s);
}

constexpr void read(span<byte> buffer, input_stream auto& s)
{
	read_raw(buffer, s);
}

template <ranges::output_range<byte> R>
constexpr void read(R&& r, input_stream auto& s)
{
	read_raw(forward<R>(r), s);
}

template <typename T>
requires integral<T> && (sizeof(T) == 1)
constexpr void read(T& object, input_stream auto& s)
{
	read_raw(object, s);
}

constexpr void read(byte& object, input_context auto& ctx)
{
	read(object, ctx.get_stream());
}

constexpr void read(span<byte> buffer, input_context auto& ctx)
{
	read(buffer, ctx.get_stream());
}

template <ranges::output_range<byte> R>
constexpr void read(R&& r, input_context auto& ctx)
{
	read(forward<R>(r), ctx.get_stream());
}

constexpr void read(bool& object, input_context auto& ctx)
{
	byte temp_byte;
	span<byte> buffer{&temp_byte, 1};
	read(buffer, ctx);
	auto integer_value = to_integer<uint_fast8_t>(temp_byte);
	object = integer_value;
}

/*constexpr*/ void read(integral auto& object, input_context auto& ctx)
{
	array<byte, sizeof(object)> buffer;
	read(buffer, ctx);
	Utilities::ToNative(buffer, ctx.get_format(), object);
}

/*constexpr*/ void read(floating_point auto& object, input_context auto& ctx)
{
	array<byte, sizeof(object)> buffer;
	read(buffer, ctx);
	Utilities::ToNative(buffer, ctx.get_format(), object);
}

template <typename I, typename... Args>
requires input_stream<I> || input_context<I>
constexpr void ReadCustomizationPoint::operator()(auto& object, I& i,
	Args&&... args) const
{
	read(object, i, forward<Args>(args)...);
}

}
}
