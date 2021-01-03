/// \file
/// \brief Internal header file that describes the write customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "context_concepts.h"

namespace std::io
{

template <typename T, typename O, typename... Args>
concept CustomlyWritableTo =
	(output_stream<O> || output_context<O>) &&
	requires(const T object, O& o, Args&&... args)
	{
		object.write(o, forward<Args>(args)...);
	};

namespace CustomizationPoints
{

template <typename T, typename O, typename... Args>
requires CustomlyWritableTo<T, O, Args...>
constexpr void write(const T& object, O& o, Args&&... args);

constexpr void write(byte object, output_stream auto& s);

constexpr void write(span<const byte> buffer, output_stream auto& s);

template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, byte>
constexpr void write(R&& r, output_stream auto& s);

template <typename T>
requires (integral<T> || is_enum_v<T>) && (sizeof(T) == 1)
constexpr void write(T object, output_stream auto& s);

constexpr void write(byte object, output_context auto& ctx);

constexpr void write(span<const byte> buffer, output_context auto& ctx);

template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, byte>
constexpr void write(R&& r, output_context auto& ctx);

constexpr void write(bool object, output_context auto& ctx);

template <typename T>
requires integral<T> || is_enum_v<T>
/*constexpr*/ void write(T object, output_context auto& ctx);

/*constexpr*/ void write(floating_point auto object, output_context auto& ctx);

struct WriteCustomizationPoint
{
	template <typename O, typename... Args>
	requires output_stream<O> || output_context<O>
	constexpr void operator()(const auto& object, O& o, Args&&... args) const;
};

}

inline constexpr CustomizationPoints::WriteCustomizationPoint write;

template <typename T, typename O, typename... Args>
concept writable_to =
	(output_stream<O> || output_context<O>) &&
	requires(const T& object, O& o, Args&&... args)
	{
		io::write(object, o, forward<Args>(args)...);
	};

}

#include "write.hpp"
