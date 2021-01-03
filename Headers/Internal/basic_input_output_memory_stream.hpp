/// \file
/// \brief Internal header file that contains implementation of the
/// basic_input_output_memory_stream class template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

template <typename Container>
constexpr basic_input_output_memory_stream<Container>::
	basic_input_output_memory_stream(const Container& c)
	: m_buffer{c}
{
}

template <typename Container>
constexpr basic_input_output_memory_stream<Container>::
	basic_input_output_memory_stream(Container&& c)
	: m_buffer{move(c)}
{
}

template <typename Container>
constexpr streamsize basic_input_output_memory_stream<Container>::read_some(
	span<byte> buffer)
{
	return Utilities::ReadSome(m_buffer, this->m_position, buffer);
}

template <typename Container>
constexpr streamsize basic_input_output_memory_stream<Container>::write_some(
	span<const byte> buffer)
{
	return Utilities::WriteSomeDynamic(m_buffer, this->m_position, buffer);
}

template <typename Container>
constexpr const Container& basic_input_output_memory_stream<Container>::
	get_buffer() const & noexcept
{
	return m_buffer;
}

template <typename Container>
constexpr Container basic_input_output_memory_stream<Container>::get_buffer() &&
	noexcept
{
	return move(m_buffer);
}

template <typename Container>
constexpr void basic_input_output_memory_stream<Container>::set_buffer(
	const Container& new_buffer)
{
	m_buffer = new_buffer;
	this->m_position = 0;
}

template <typename Container>
constexpr void basic_input_output_memory_stream<Container>::set_buffer(
	Container&& new_buffer)
{
	m_buffer = move(new_buffer);
	this->m_position = 0;
}

template <typename Container>
constexpr void basic_input_output_memory_stream<Container>::reset_buffer()
	noexcept
{
	m_buffer.clear();
	this->m_position = 0;
}

}
