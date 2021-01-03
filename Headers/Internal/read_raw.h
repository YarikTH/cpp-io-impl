/// \file
/// \brief Internal header file that describes the read_raw customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{
namespace CustomizationPoints
{

constexpr void read_raw(byte& object, input_stream auto& s);

constexpr void read_raw(span<byte> buffer, input_stream auto& s);

template <ranges::output_range<byte> R>
constexpr void read_raw(R&& r, input_stream auto& s);

template <typename T>
requires integral<T> && (sizeof(T) == 1)
constexpr void read_raw(T& object, input_stream auto& s);

struct ReadRawCustomizationPoint
{
	constexpr void operator()(auto& object, input_stream auto& s) const;
};

}

inline constexpr CustomizationPoints::ReadRawCustomizationPoint read_raw;

}

#include "read_raw.hpp"
