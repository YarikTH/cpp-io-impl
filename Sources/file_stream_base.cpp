/// \file
/// \brief Source file that contains implementation of the file_stream_base
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/file_stream_base.h>

namespace std::io
{

file_stream_base::file_stream_base(const filesystem::path& file_name, mode m,
	creation c)
	: m_file{file_name, m, c}
{
}

file_stream_base::file_stream_base(native_handle_type handle)
	: m_file{handle}
{
}

position file_stream_base::get_position() const
{
	return m_file.get_position();
}

void file_stream_base::seek_position(position pos)
{
	m_file.seek_position(pos);
}

void file_stream_base::seek_position(offset off)
{
	m_file.seek_position(off);
}

void file_stream_base::seek_position(base_position base)
{
	m_file.seek_position(base);
}

void file_stream_base::seek_position(base_position base, offset off)
{
	m_file.seek_position(base, off);
}

void file_stream_base::flush()
{
	m_file.flush();
}

file_stream_base::native_handle_type file_stream_base::native_handle()
{
	return m_file.native_handle();
}

void file_stream_base::assign(native_handle_type handle)
{
	m_file.assign(handle);
}

file_stream_base::native_handle_type file_stream_base::release()
{
	return m_file.release();
}

}
