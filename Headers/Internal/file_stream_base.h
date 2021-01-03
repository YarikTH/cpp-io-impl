/// \file
/// \brief Internal header file that describes the file_stream_base class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "buffered_file.h"

namespace std::io
{

class file_stream_base
{
public:
	using native_handle_type = File::native_handle_type;
	
protected:
	// Construct/copy/destroy
	file_stream_base() noexcept = default;
	file_stream_base(const filesystem::path& file_name, mode m, creation c);
	file_stream_base(native_handle_type handle);
	file_stream_base(const file_stream_base&) = delete;
	file_stream_base(file_stream_base&&) = default;
	file_stream_base& operator=(const file_stream_base&) = delete;
	file_stream_base& operator=(file_stream_base&&) = default;
	
public:
	// Position
	position get_position() const;
	void seek_position(position pos);
	void seek_position(offset off);
	void seek_position(base_position base);
	void seek_position(base_position base, offset off);
	
	// Buffering
	void flush();
	
	// Native handle management
	native_handle_type native_handle();
	void assign(native_handle_type handle);
	native_handle_type release();
protected:
	BufferedFile m_file;
};

}
