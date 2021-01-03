/// \file
/// \brief Internal header file that contains implementation of the
/// output_span_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

constexpr output_span_stream::output_span_stream(span<byte> buffer) noexcept
	: m_buffer{buffer}
{
}

constexpr streamsize output_span_stream::write_some(span<const byte> buffer)
{
	return Utilities::WriteSomeFixed(m_buffer, m_position, buffer);
}

constexpr span<byte> output_span_stream::get_buffer() const noexcept
{
	return m_buffer;
}

constexpr void output_span_stream::set_buffer(span<byte> new_buffer) noexcept
{
	m_buffer = new_buffer;
	m_position = 0;
}

}
