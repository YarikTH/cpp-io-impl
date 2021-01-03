/// \file
/// \brief Internal header file that describes stream concepts.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <concepts>
#include <span>

#include "position.h"

namespace std::io
{

template <typename T>
concept input_stream = requires(T s, span<byte> buffer)
	{
		{s.read_some(buffer)} -> same_as<streamsize>;
	};

template <typename T>
concept output_stream = requires(T s, span<const byte> buffer)
	{
		{s.write_some(buffer)} -> same_as<streamsize>;
	};

template <typename T>
concept stream = input_stream<T> || output_stream<T>;

template <typename T>
concept input_output_stream = input_stream<T> && output_stream<T>;

template <typename T>
concept seekable_stream = stream<T> && requires(const T s)
	{
		{s.get_position()} -> same_as<position>;
	} && requires(T s, position pos, offset off, base_position base)
	{
		s.seek_position(pos);
		s.seek_position(off);
		s.seek_position(base);
		s.seek_position(base, off);
	};

template <typename T>
concept buffered_stream = stream<T> && requires(T s)
	{
		s.flush();
	};

}
