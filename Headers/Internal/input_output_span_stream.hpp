/// \file
/// \brief Internal header file that contains implementation of the
/// input_output_span_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "stream_utilities.h"

namespace std::io
{

constexpr input_output_span_stream::input_output_span_stream(span<byte> buffer)
	noexcept
	: m_buffer{buffer}
{
}

constexpr streamsize input_output_span_stream::read_some(span<byte> buffer)
{
	return Utilities::ReadSome(m_buffer, m_position, buffer);
}

constexpr streamsize input_output_span_stream::write_some(
	span<const byte> buffer)
{
	return Utilities::WriteSomeFixed(m_buffer, m_position, buffer);
}

constexpr span<byte> input_output_span_stream::get_buffer() const noexcept
{
	return m_buffer;
}

constexpr void input_output_span_stream::set_buffer(span<byte> new_buffer)
	noexcept
{
	m_buffer = new_buffer;
	m_position = 0;
}

}
