/// \file
/// \brief Internal header file that describes the format class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <bit>

namespace std::io
{

enum class floating_point_format
{
	iec559,
	native
};

class format final
{
public:
	// Constructor
	constexpr format(endian endianness = endian::native,
		floating_point_format float_format = floating_point_format::native)
		noexcept;
	
	// Member functions
	constexpr endian get_endianness() const noexcept;
	constexpr void set_endianness(endian new_endianness) noexcept;
	constexpr floating_point_format get_floating_point_format() const noexcept;
	constexpr void set_floating_point_format(floating_point_format new_format)
		noexcept;
	
	// Equality
	friend constexpr bool operator==(const format& lhs, const format& rhs)
		noexcept = default;
private:
	endian m_endianness;
	floating_point_format m_float_format;
};

}

#include "format.hpp"
