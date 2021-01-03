/// \file
/// \brief Internal header file that describes the
/// basic_input_output_memory_stream class template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

template <typename Container>
class basic_input_output_memory_stream final :
	public PositionHelper<basic_input_output_memory_stream<Container>,
		typename Container::difference_type>
{
public:
	// Constructors
	constexpr basic_input_output_memory_stream() = default;
	constexpr basic_input_output_memory_stream(const Container& c);
	constexpr basic_input_output_memory_stream(Container&& c);
	
	// Reading
	constexpr streamsize read_some(span<byte> buffer);
	
	// Writing
	constexpr streamsize write_some(span<const byte> buffer);
	
	// Buffer management
	constexpr const Container& get_buffer() const & noexcept;
	constexpr Container get_buffer() && noexcept;
	constexpr void set_buffer(const Container& new_buffer);
	constexpr void set_buffer(Container&& new_buffer);
	constexpr void reset_buffer() noexcept;
private:
	Container m_buffer;
};

using input_output_memory_stream = basic_input_output_memory_stream<vector<
	byte>>;

}

#include "basic_input_output_memory_stream.hpp"
