/// \file
/// \brief Internal header file that contains implementation of the write_raw
/// customization point.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{
namespace CustomizationPoints
{

constexpr void write_raw(byte object, output_stream auto& s)
{
	span<const byte> buffer{&object, 1};
	write_raw(buffer, s);
}

constexpr void write_raw(span<const byte> buffer, output_stream auto& s)
{
	auto bytes_to_write = ranges::ssize(buffer);
	streamsize bytes_written;
	while (bytes_to_write > 0)
	{
		try
		{
			bytes_written = s.write_some(buffer);
		}
		catch (io_error& e)
		{
			if (e.code() != io_errc::interrupted)
			{
				throw;
			}
			continue;
		}
		bytes_to_write -= bytes_written;
		buffer = buffer.last(bytes_to_write);
	}
}

template <ranges::input_range R>
requires same_as<ranges::range_value_t<R>, byte>
constexpr void write_raw(R&& r, output_stream auto& s)
{
	if constexpr (ranges::contiguous_range<R>)
	{
		span<const byte> buffer{ranges::data(r), ranges::size(r)};
		write_raw(buffer, s);
		return;
	}
	else
	{
		auto i = ranges::begin(r);
		auto last = ranges::end(r);
		while (i != last)
		{
			byte b = *i;
			write_raw(b, s);
			++i;
		}
	}
}

template <typename T>
requires integral<T> && (sizeof(T) == 1)
constexpr void write_raw(T object, output_stream auto& s)
{
	write_raw(static_cast<byte>(object), s);
}

constexpr void WriteRawCustomizationPoint::operator()(const auto& object,
	output_stream auto& s) const
{
	write_raw(object, s);
}

}
}
