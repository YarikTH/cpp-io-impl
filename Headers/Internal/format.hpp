/// \file
/// \brief Internal header file that contains implementation of the format
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

constexpr format::format(endian endianness, floating_point_format float_format)
	noexcept
	: m_endianness{endianness},
	m_float_format{float_format}
{
}

constexpr endian format::get_endianness() const noexcept
{
	return m_endianness;
}

constexpr void format::set_endianness(endian new_endianness) noexcept
{
	m_endianness = new_endianness;
}

constexpr floating_point_format format::get_floating_point_format() const
	noexcept
{
	return m_float_format;
}

constexpr void format::set_floating_point_format(
	floating_point_format new_format) noexcept
{
	m_float_format = new_format;
}

}
