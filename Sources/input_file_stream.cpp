/// \file
/// \brief Source file that contains implementation of the input_file_stream
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/input_file_stream.h>

namespace std::io
{

input_file_stream::input_file_stream(const filesystem::path& file_name)
	: file_stream_base{file_name, mode::read, creation::open_existing}
{
}

input_file_stream::input_file_stream(native_handle_type handle)
	: file_stream_base{handle}
{
}

streamsize input_file_stream::read_some(span<byte> buffer)
{
	return m_file.read_some(buffer);
}

}
