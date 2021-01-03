/// \file
/// \brief Internal header file that describes the utility functions for POSIX
/// IO.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <ios>
#include <span>
#include <filesystem>

#include "../position.h"
#include "../file_options.h"

#include "Platform.h"

namespace std::io::POSIX
{

using NativeHandle = int; ///< Native POSIX file handle.
constexpr NativeHandle InvalidNativeHandle = -1; ///< Invalid POSIX file handle.

/// \brief Opens or creates a file and returns the native handle to it.
/// \param[in] file_name Name of the file to open.
/// \param[in] m TODO
/// \param[in] c TODO
/// \return Native handle to the file.
/// \throw TODO
NativeHandle OpenFile(const filesystem::path& file_name, mode m, creation c);

/// \brief Closes the native handle if it is valid.
/// \param[in] handle Native handle to close.
void CloseFile(NativeHandle handle) noexcept;

/// \brief Returns current position in the file.
/// \param[in] handle Native handle to inspect.
/// \return Current position in the file.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
position GetPosition(NativeHandle handle);

/// \brief Sets the position in the file to the given one.
/// \param[in] handle Native handle to work with.
/// \param[in] pos Position to set.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
void SeekPosition(NativeHandle handle, position pos);

/// \brief Sets the position in the file to the given offset relative to the
/// current position in the file.
/// \param[in] handle Native handle to work with.
/// \param[in] off Offset to seek by.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
void SeekPosition(NativeHandle handle, offset off);

/// \brief Sets the position in the file to the given base position.
/// \param[in] handle Native handle to work with.
/// \param[in] base Base position to set.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
void SeekPosition(NativeHandle handle, base_position base);

/// \brief Sets the position in the file specified by the given base position
/// and offset.
/// \param[in] handle Native handle to work with.
/// \param[in] base Base position to use for seeking.
/// \param[in] off Offset to apply to the given base position.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
void SeekPosition(NativeHandle handle, base_position base, offset off);

/// \brief Reads zero or more bytes from the file to the given buffer and
/// advances file position by the amount of bytes read.
/// \param[in] handle Native handle to read from.
/// \param[in,out] buffer Buffer to write to.
/// \return Amount of bytes read.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
streamsize ReadSome(NativeHandle handle, span<byte> buffer);

/// \brief Writes zero or more bytes to the file from the given buffer and
/// advances file position by the amount of bytes written.
/// \param[in] handle Native handle to write to.
/// \param[in] buffer Buffer to read from.
/// \return Amount of bytes written.
/// \throw std::io::io_error In case of documented error.
/// \throw std::system_error In case of undocumented error.
streamsize WriteSome(NativeHandle handle, span<const byte> buffer);

/// \brief Returns the optimal buffer size for the given native handle.
/// \param[in] handle Native handle to inspect.
/// \return Optimal buffer size.
/// \throw std::system_error In case of error.
size_t GetBufferSize(NativeHandle handle);

}
