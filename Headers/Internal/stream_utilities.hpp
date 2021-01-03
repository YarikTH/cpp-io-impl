/// \file
/// \brief Internal header file that contains implementation of the utility
/// functions for streams.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <algorithm>

#include "io_error.h"

namespace std::io::Utilities
{

template <typename Position>
constexpr auto GetBytesToTransfer(Position stream_buffer_size,
	Position stream_position, ptrdiff_t transfer_buffer_size) noexcept
{
	// This doesn't overflow and guarantees that position + result fits into the
	// type of position. See 2nd part of the equation. We are already
	// subtracting position so adding it back gives us buffer size which is the
	// same type as position.
	auto temp_result = min(transfer_buffer_size,
		stream_buffer_size - stream_position);
	// Make sure we fit into std::streamsize.
	if constexpr (sizeof(temp_result) > sizeof(streamsize))
	{
		temp_result = min(temp_result, numeric_limits<streamsize>::max());
	}
	// But we still need to make sure that we fit into std::streamoff.
	if constexpr (sizeof(temp_result) <= sizeof(streamoff))
	{
		return temp_result;
	}
	// Again, left side doesn't overflow because it was very nicely calculated.
	if (stream_position + temp_result > numeric_limits<streamoff>::max())
	{
		temp_result = numeric_limits<streamoff>::max() - stream_position;
	}
	return temp_result;
}

template <typename Buffer, typename Position>
constexpr streamsize ReadSome(const Buffer& in_buffer, Position& pos,
	span<byte> out_buffer)
{
	auto out_size = ranges::ssize(out_buffer);
	if (out_size == 0)
	{
		return 0;
	}
	auto in_size = ranges::ssize(in_buffer);
	if (pos >= in_size)
	{
		// Already at the end of stream.
		return 0;
	}
	if (pos == position::max().value())
	{
		// Already at the maximum position supported by implementation.
		throw io_error{"ReadSome", io_errc::value_too_large};
	}
	auto bytes_to_read = GetBytesToTransfer(in_size, pos, out_size);
	auto end_position = pos + bytes_to_read;
	auto first = ranges::begin(in_buffer) + pos;
	auto last = ranges::begin(in_buffer) + end_position;
	ranges::copy(first, last, ranges::begin(out_buffer));
	pos = end_position;
	return bytes_to_read;
}

template <typename Buffer, typename Position>
constexpr streamsize WriteSomeFixed(Buffer& out_buffer, Position& pos,
	span<const byte> in_buffer)
{
	auto in_size = ranges::ssize(in_buffer);
	if (in_size == 0)
	{
		return 0;
	}
	auto out_size = ranges::ssize(out_buffer);
	if (pos >= out_size)
	{
		// Already at the end of stream.
		throw io_error{"WriteSomeFixed", io_errc::file_too_large};
	}
	if (pos == position::max().value())
	{
		// Already at the maximum position supported by the implementation.
		throw io_error{"WriteSomeFixed", io_errc::file_too_large};
	}
	auto bytes_to_write = GetBytesToTransfer(out_size, pos, in_size);
	auto end_position = pos + bytes_to_write;
	auto in_first = ranges::begin(in_buffer);
	auto in_last = in_first + bytes_to_write;
	auto out_first = ranges::begin(out_buffer) + pos;
	ranges::copy(in_first, in_last, out_first);
	pos = end_position;
	return bytes_to_write;
}

template <typename Buffer, typename Position>
constexpr streamsize WriteSomeDynamic(Buffer& out_buffer, Position& pos,
	span<const byte> in_buffer)
{
	auto in_size = ranges::ssize(in_buffer);
	if (in_size == 0)
	{
		return 0;
	}
	auto max_out_size = static_cast<typename Buffer::difference_type>(
		out_buffer.max_size());
	if (pos >= max_out_size)
	{
		throw io_error{"WriteSomeDynamic", io_errc::file_too_large};
	}
	if (pos == position::max().value())
	{
		// Already at the maximum position supported by the implementation.
		throw io_error{"WriteSomeDynamic", io_errc::file_too_large};
	}
	auto out_size = ranges::ssize(out_buffer);
	if (pos < out_size)
	{
		// Try to write as much bytes as possible before expanding buffer.
		auto bytes_to_write = GetBytesToTransfer(out_size, pos, in_size);
		auto end_position = pos + bytes_to_write;
		auto in_first = ranges::begin(in_buffer);
		auto in_last = in_first + bytes_to_write;
		auto out_first = ranges::begin(out_buffer) + pos;
		ranges::copy(in_first, in_last, out_first);
		pos = end_position;
		return bytes_to_write;
	}
	auto bytes_to_write = GetBytesToTransfer(max_out_size, pos, in_size);
	auto end_position = pos + bytes_to_write;
	out_buffer.resize(end_position);
	auto in_first = ranges::begin(in_buffer);
	auto in_last = in_first + bytes_to_write;
	auto out_first = ranges::begin(out_buffer) + pos;
	ranges::copy(in_first, in_last, out_first);
	pos = end_position;
	return bytes_to_write;
}

}
