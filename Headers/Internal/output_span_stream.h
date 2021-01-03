/// \file
/// \brief Internal header file that describes the output_span_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

class output_span_stream final :
	public PositionHelper<output_span_stream, ptrdiff_t>
{
public:
	// Constructors
	constexpr output_span_stream() noexcept = default;
	constexpr output_span_stream(span<byte> buffer) noexcept;
	
	// Writing
	constexpr streamsize write_some(span<const byte> buffer);
	
	// Buffer management
	constexpr span<byte> get_buffer() const noexcept;
	constexpr void set_buffer(span<byte> new_buffer) noexcept;
private:
	span<byte> m_buffer;
};

}

#include "output_span_stream.hpp"
