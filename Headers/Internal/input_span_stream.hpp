/// \file
/// \brief Internal header file that contains implementation of the
/// input_span_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "stream_utilities.h"

namespace std::io
{

constexpr input_span_stream::input_span_stream(span<const byte> buffer) noexcept
	: m_buffer{buffer}
{
}

constexpr streamsize input_span_stream::read_some(span<byte> buffer)
{
	return Utilities::ReadSome(m_buffer, m_position, buffer);
}

constexpr span<const byte> input_span_stream::get_buffer() const noexcept
{
	return m_buffer;
}

constexpr void input_span_stream::set_buffer(span<const byte> new_buffer)
	noexcept
{
	m_buffer = new_buffer;
	m_position = 0;
}

}
