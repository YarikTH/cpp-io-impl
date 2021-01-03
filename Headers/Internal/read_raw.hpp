/// \file
/// \brief Internal header file that contains implementation of the read_raw
/// customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "io_error.h"

namespace std::io
{
namespace CustomizationPoints
{

constexpr void read_raw(byte& object, input_stream auto& s)
{
	span<byte> buffer{&object, 1};
	read_raw(buffer, s);
}

constexpr void read_raw(span<byte> buffer, input_stream auto& s)
{
	auto bytes_to_read = ranges::ssize(buffer);
	streamsize bytes_read;
	while (bytes_to_read > 0)
	{
		try
		{
			bytes_read = s.read_some(buffer);
		}
		catch (io_error& e)
		{
			if (e.code() != io_errc::interrupted)
			{
				throw;
			}
			continue;
		}
		if (bytes_read == 0)
		{
			throw io_error{"read: Reached end of stream",
				io_errc::reached_end_of_file};
		}
		bytes_to_read -= bytes_read;
		buffer = buffer.last(bytes_to_read);
	}
}

template <ranges::output_range<byte> R>
constexpr void read_raw(R&& r, input_stream auto& s)
{
	if constexpr (ranges::contiguous_range<R>)
	{
		span<byte> buffer{ranges::data(r), ranges::size(r)};
		read_raw(buffer, s);
		return;
	}
	else
	{
		auto i = ranges::begin(r);
		auto last = ranges::end(r);
		while (i != last)
		{
			byte b;
			read_raw(b, s);
			*i = b;
			++i;
		}
	}
}

template <typename T>
requires integral<T> && (sizeof(T) == 1)
constexpr void read_raw(T& object, input_stream auto& s)
{
	byte temp_byte;
	read_raw(temp_byte, s);
	object = to_integer<T>(temp_byte);
}

constexpr void ReadRawCustomizationPoint::operator()(auto& object,
	input_stream auto& s) const
{
	read_raw(object, s);
}

}
}
