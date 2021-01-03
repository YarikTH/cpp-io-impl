/// \file
/// \brief Internal header file that contains implementation of the write
/// customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "write_raw.h"

namespace std::io
{
namespace CustomizationPoints
{

template <typename T, typename O, typename... Args>
requires CustomlyWritableTo<T, O, Args...>
constexpr void write(const T& object, O& o, Args&&... args)
{
	object.write(o, forward<Args>(args)...);
}

constexpr void write(byte object, output_stream auto& s)
{
	write_raw(object, s);
}

constexpr void write(span<const byte> buffer, output_stream auto& s)
{
	write_raw(buffer, s);
}

template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, byte>
constexpr void write(R&& r, output_stream auto& s)
{
	write_raw(forward<R>(r), s);
}

template <typename T>
requires (integral<T> || is_enum_v<T>) && (sizeof(T) == 1)
constexpr void write(T object, output_stream auto& s)
{
	write_raw(static_cast<byte>(object), s);
}

constexpr void write(byte object, output_context auto& ctx)
{
	write(object, ctx.get_stream());
}

constexpr void write(span<const byte> buffer, output_context auto& ctx)
{
	write(buffer, ctx.get_stream());
}

template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, byte>
constexpr void write(R&& r, output_context auto& ctx)
{
	write(forward<R>(r), ctx.get_stream());
}

constexpr void write(bool object, output_context auto& ctx)
{
	// Integral promotion to int per [conv.prom] then explicit cast of that to
	// unsigned char to silence compiler warnings.
	byte temp_byte = byte{static_cast<unsigned char>(static_cast<int>(object))};
	span<const byte> buffer{&temp_byte, 1};
	write(buffer, ctx);
}

template <typename T>
requires integral<T> || is_enum_v<T>
/*constexpr*/ void write(T object, output_context auto& ctx)
{
	array<byte, sizeof(object)> buffer;
	Utilities::FromNative(object, ctx.get_format(), buffer);
	write(buffer, ctx);
}

/*constexpr*/ void write(floating_point auto object, output_context auto& ctx)
{
	array<byte, sizeof(object)> buffer;
	Utilities::FromNative(object, ctx.get_format(), buffer);
	write(buffer, ctx);
}

template <typename O, typename... Args>
requires output_stream<O> || output_context<O>
constexpr void WriteCustomizationPoint::operator()(const auto& object, O& o,
	Args&&... args) const
{
	write(object, o, forward<Args>(args)...);
}

}
}
