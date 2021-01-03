/// \file
/// \brief Source file that contains implementation of the
/// input_output_file_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/input_output_file_stream.h>

namespace std::io
{

input_output_file_stream::input_output_file_stream(
	const filesystem::path& file_name, creation c)
	: file_stream_base{file_name, mode::write, c}
{
}

input_output_file_stream::input_output_file_stream(native_handle_type handle)
	: file_stream_base{handle}
{
}

streamsize input_output_file_stream::read_some(span<byte> buffer)
{
	return m_file.read_some(buffer);
}

streamsize input_output_file_stream::write_some(span<const byte> buffer)
{
	return m_file.write_some(buffer);
}

}
