/// \file
/// \brief Internal header file that describes the read customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "context_concepts.h"

namespace std::io
{

template <typename T, typename I, typename... Args>
concept CustomlyReadableFrom =
	(input_stream<I> || input_context<I>) &&
	requires(T object, I& i, Args&&... args)
	{
		object.read(i, forward<Args>(args)...);
	};

namespace CustomizationPoints
{

template <typename T, typename I, typename... Args>
requires CustomlyReadableFrom<T, I, Args...>
constexpr void read(T& object, I& i, Args&&... args);

constexpr void read(byte& object, input_stream auto& s);

constexpr void read(span<byte> buffer, input_stream auto& s);

template <ranges::output_range<byte> R>
constexpr void read(R&& r, input_stream auto& s);

template <typename T>
requires integral<T> && (sizeof(T) == 1)
constexpr void read(T& object, input_stream auto& s);

constexpr void read(byte& object, input_context auto& ctx);

constexpr void read(span<byte> buffer, input_context auto& ctx);

template <ranges::output_range<byte> R>
constexpr void read(R&& r, input_context auto& ctx);

constexpr void read(bool& object, input_context auto& ctx);

/*constexpr*/ void read(integral auto& object, input_context auto& ctx);

/*constexpr*/ void read(floating_point auto& object, input_context auto& ctx);

struct ReadCustomizationPoint
{
	template <typename I, typename... Args>
	requires input_stream<I> || input_context<I>
	constexpr void operator()(auto& object, I& i, Args&&... args) const;
};

}

inline constexpr CustomizationPoints::ReadCustomizationPoint read;

template <typename T, typename I, typename... Args>
concept readable_from =
	(input_stream<I> || input_context<I>) &&
	requires(T& object, I& i, Args&&... args)
	{
		io::read(object, i, forward<Args>(args)...);
	};

}

#include "read.hpp"
