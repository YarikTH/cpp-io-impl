/// \file
/// \brief Internal header file that contains implementation of the
/// PositionHelper class template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "io_error.h"

namespace std::io
{

template <typename Stream, typename Position>
constexpr PositionHelper<Stream, Position>::PositionHelper() noexcept
	: m_position{0}
{
}

template <typename Stream, typename Position>
constexpr position PositionHelper<Stream, Position>::get_position() const
	noexcept
{
	return position{m_position};
}

template <typename Stream, typename Position>
constexpr void PositionHelper<Stream, Position>::seek_position(position pos)
{
	auto raw_position = pos.value();
	if (raw_position < 0)
	{
		throw io_error{"seek_position", io_errc::invalid_argument};
	}
	if (raw_position > std::numeric_limits<Position>::max())
	{
		throw io_error{"seek_position", io_errc::value_too_large};
	}
	m_position = raw_position;
}

template <typename Stream, typename Position>
constexpr void PositionHelper<Stream, Position>::seek_position(offset off)
{
	this->seek_position(MovePosition(m_position, off));
}

template <typename Stream, typename Position>
constexpr void PositionHelper<Stream, Position>::seek_position(
	base_position base) noexcept
{
	switch (base)
	{
		case base_position::beginning:
		{
			m_position = 0;
			return;
		}
		case base_position::current:
		{
			return;
		}
		case base_position::end:
		{
			const auto& buffer = static_cast<Stream*>(this)->get_buffer();
			if constexpr (sizeof(Position) <= sizeof(streamoff))
			{
				m_position = ranges::ssize(buffer);
				return;
			}
			m_position = min(ranges::ssize(buffer), position::max().value());
			return;
		}
		default:
		{
			return;
		}
	}
}

template <typename Stream, typename Position>
constexpr void PositionHelper<Stream, Position>::seek_position(
	base_position base, offset off)
{
	switch (base)
	{
		case base_position::beginning:
		{
			this->seek_position(position{off});
			return;
		}
		case base_position::current:
		{
			this->seek_position(off);
			return;
		}
		case base_position::end:
		{
			const auto& buffer = static_cast<Stream*>(this)->get_buffer();
			this->seek_position(MovePosition(ranges::ssize(buffer), off));
			return;
		}
		default:
		{
			return;
		}
	}
}

template <typename Stream, typename Position>
constexpr position PositionHelper<Stream, Position>::MovePosition(Position pos,
	offset off)
{
	using ResultType = decltype(pos + off.value());
	ResultType temp_position = pos;
	ResultType temp_offset = off.value();
	// Check if adding offset to position overflows.
	// Position can't be negative so no check for underflow here.
	if (temp_offset > numeric_limits<ResultType>::max() - temp_position)
	{
		throw io_error{"MovePosition", io_errc::value_too_large};
	}
	ResultType result = temp_position + temp_offset;
	if constexpr (sizeof(ResultType) == sizeof(streamoff))
	{
		return position{result};
	}
	if (result > numeric_limits<streamoff>::max())
	{
		throw io_error{"MovePosition", io_errc::value_too_large};
	}
	return position{result};
}

}
