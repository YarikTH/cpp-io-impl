/// \file
/// \brief Internal header file that describes the BufferedFile class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <vector>

#include "input_output_span_stream.h"
#include "file.h"

namespace std::io
{

/// \brief A wrapper around unbuffered file that provides buffering.
/// \details This class provides the full API to do buffered IO on files.

class BufferedFile final
{
public:
	using native_handle_type = File::native_handle_type;
	
	// Construct/copy/destroy
	BufferedFile() noexcept;
	BufferedFile(const filesystem::path& file_name, mode m, creation c);
	BufferedFile(native_handle_type handle);
	BufferedFile(const BufferedFile&) = delete;
	BufferedFile(BufferedFile&& other);
	~BufferedFile();
	BufferedFile& operator=(const BufferedFile&) = delete;
	BufferedFile& operator=(BufferedFile&& other);
	
	// Position
	position get_position() const;
	void seek_position(position pos);
	void seek_position(offset off);
	void seek_position(base_position base);
	void seek_position(base_position base, offset off);
	
	// Buffering
	void flush();
	
	// Reading
	streamsize read_some(span<byte> buffer);
	
	// Writing
	streamsize write_some(span<const byte> buffer);
	
	// Native handle management
	native_handle_type native_handle();
	void assign(native_handle_type handle);
	native_handle_type release();
private:
	File m_file; ///< Unbuffered file.
	vector<byte> m_buffer_storage; ///< Byte storage for the buffer.
	input_output_span_stream m_buffer_stream; ///< Buffer stream.
	mode m_buffer_mode; ///< Mode of the buffer.
	
	/// \brief Returns whether the buffer is empty.
	/// \return True if the buffer is empty, false otherwise.
	bool IsBufferEmpty() const noexcept;
	
	/// \brief Setups a new read buffer.
	/// \note This will call read_some on file to fill the buffer.
	void SetNewReadBuffer();
	
	/// \brief Setups a new write buffer.
	void SetNewWriteBuffer();
	
	/// \brief Returns the current offset in the file system sector.
	/// \return Current offset in the file system sector.
	streamoff GetSectorOffset() const;
	
	/// \brief Returns the byte span that starts at the current position and
	/// ends on a sector boundary.
	/// \return Byte span that starts at the current position and
	/// ends on a sector boundary.
	span<byte> GetBufferUntilTheEndOfSector();
};

}
