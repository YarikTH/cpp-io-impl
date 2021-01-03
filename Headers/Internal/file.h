/// \file
/// \brief Internal header file that describes the File class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "position.h"
#include "basic_file.h"

namespace std::io
{

/// \brief A regular unbuffered file.
/// \details This class provides full interface to work with regular files
/// without buffering.

class File final : public BasicFile
{
public:
	using BasicFile::native_handle_type;
	
	// Constructors
	File() noexcept = default;
	File(const filesystem::path& file_name, mode m, creation c);
	File(native_handle_type handle);
	
	// Position
	position get_position() const;
	void seek_position(position pos);
	void seek_position(offset off);
	void seek_position(base_position base);
	void seek_position(base_position base, offset off);
	
	// Reading
	streamsize read_some(span<byte> buffer);
	
	// Writing
	streamsize write_some(span<const byte> buffer);
};

}
