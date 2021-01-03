/// \file
/// \brief Internal header file that describes the utility functions for
/// streams.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io::Utilities
{

/// \brief Returns the amount of bytes that can be transferred safely in one
/// operation.
/// \tparam Position Type of the stream position.
/// \param[in] stream_buffer_size Size of the stream buffer.
/// \param[in] stream_position Stream position to start transfer from.
/// \param[in] transfer_buffer_size Size of the buffer to transfer bytes with.
/// \return Amount of bytes to transfer.
template <typename Position>
constexpr auto GetBytesToTransfer(Position stream_buffer_size,
	Position stream_position, ptrdiff_t transfer_buffer_size) noexcept;

/// \brief Reads zero or more bytes from the stream to the given buffer and
/// advances stream position by the amount of bytes read.
/// \tparam Buffer Type of the stream buffer.
/// \tparam Position Type of the stream position.
/// \param[in] in_buffer Stream buffer to read from.
/// \param[in,out] pos Stream position to start reading from.
/// \param[in,out] out_buffer Buffer to write to.
/// \return Amount of bytes read.
/// \throw std::io::io_error If stream position at the start of reading has
/// maximum value supported by the implementation.
/// \note This function finishes if the given buffer is filled or the end of
/// stream is reached.
template <typename Buffer, typename Position>
constexpr streamsize ReadSome(const Buffer& in_buffer, Position& pos,
	span<byte> out_buffer);

/// \brief Writes zero or more bytes to the fixed-size stream and advances
/// stream position by the amount of bytes written.
/// \tparam Buffer Type of the stream buffer.
/// \tparam Position Type of the stream position.
/// \param[in,out] out_buffer Stream buffer to write to.
/// \param[in,out] pos Stream position to start writing to.
/// \param[in] in_buffer Buffer to read from.
/// \return Amount of bytes written.
/// \throw std::io::io_error If stream position at the start of writing is
/// already at the end of the stream buffer or has maximum value supported by
/// the implementation.
/// \note This function finishes if the given buffer is fully written or the end
/// of stream is reached.
template <typename Buffer, typename Position>
constexpr streamsize WriteSomeFixed(Buffer& out_buffer, Position& pos,
	span<const byte> in_buffer);

/// \brief Writes zero or more bytes to the dynamic-size stream and advances
/// stream position by the amount of bytes written.
/// \tparam Buffer Type of the stream buffer.
/// \tparam Position Type of the stream position.
/// \param[in,out] out_buffer Stream buffer to write to.
/// \param[in,out] pos Stream position to start writing to.
/// \param[in] in_buffer Buffer to read from.
/// \return Amount of bytes written.
/// \throw std::io::io_error If stream position at the start of writing is
/// already at the end of the stream buffer of maximum size or has maximum value
/// supported by the implementation.
/// \throw std::bad_alloc If stream position at the start of writing is at the
/// end of the stream buffer and there was not enough memory to allocate the
/// stream buffer of required size.
template <typename Buffer, typename Position>
constexpr streamsize WriteSomeDynamic(Buffer& out_buffer, Position& pos,
	span<const byte> in_buffer);

}

#include "stream_utilities.hpp"
