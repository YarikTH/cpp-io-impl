/// \file
/// \brief Source file that contains implementation of the output_file_stream
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/output_file_stream.h>

namespace std::io
{

output_file_stream::output_file_stream(const filesystem::path& file_name,
	creation c)
	: file_stream_base{file_name, mode::write, c}
{
}

output_file_stream::output_file_stream(native_handle_type handle)
	: file_stream_base{handle}
{
}

streamsize output_file_stream::write_some(span<const byte> buffer)
{
	return m_file.write_some(buffer);
}

}
