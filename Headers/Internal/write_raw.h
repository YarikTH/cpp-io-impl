/// \file
/// \brief Internal header file that describes the write_raw customization
/// point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "stream_concepts.h"

namespace std::io
{
namespace CustomizationPoints
{

constexpr void write_raw(byte object, output_stream auto& s);

constexpr void write_raw(span<const byte> buffer, output_stream auto& s);

template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, byte>
constexpr void write_raw(R&& r, output_stream auto& s);

template <typename T>
requires integral<T> && (sizeof(T) == 1)
constexpr void write_raw(T object, output_stream auto& s);

struct WriteRawCustomizationPoint
{
	constexpr void operator()(const auto& object, output_stream auto& s) const;
};

}

inline constexpr CustomizationPoints::WriteRawCustomizationPoint write_raw;

}

#include "write_raw.hpp"
