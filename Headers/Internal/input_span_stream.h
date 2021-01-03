/// \file
/// \brief Internal header file that describes the input_span_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "position_helper.h"

namespace std::io
{

class input_span_stream final :
	public PositionHelper<input_span_stream, ptrdiff_t>
{
public:
	// Constructors
	constexpr input_span_stream() noexcept = default;
	constexpr input_span_stream(span<const byte> buffer) noexcept;
	
	// Reading
	constexpr streamsize read_some(span<byte> buffer);
	
	// Buffer management
	constexpr span<const byte> get_buffer() const noexcept;
	constexpr void set_buffer(span<const byte> new_buffer) noexcept;
private:
	span<const byte> m_buffer;
};

}

#include "input_span_stream.hpp"
