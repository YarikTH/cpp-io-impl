/// \file
/// \brief Internal header file that describes the input_output_span_stream
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <span>

#include "position_helper.h"

namespace std::io
{

class input_output_span_stream final :
	public PositionHelper<input_output_span_stream, ptrdiff_t>
{
public:
	// Constructors
	constexpr input_output_span_stream() noexcept = default;
	constexpr input_output_span_stream(span<byte> buffer) noexcept;
	
	// Reading
	constexpr streamsize read_some(span<byte> buffer);
	
	// Writing
	constexpr streamsize write_some(span<const byte> buffer);
	
	// Buffer management
	constexpr span<byte> get_buffer() const noexcept;
	constexpr void set_buffer(span<byte> new_buffer) noexcept;
private:
	span<byte> m_buffer;
};

}

#include "input_output_span_stream.hpp"
