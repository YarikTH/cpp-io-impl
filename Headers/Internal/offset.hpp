/// \file
/// \brief Internal header file that contains implementation of the offset
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

constexpr offset::offset(streamoff off) noexcept
	: m_offset{off}
{
}

constexpr streamoff offset::value() const noexcept
{
	return m_offset;
}

constexpr offset offset::operator+() const noexcept
{
	return *this;
}

constexpr offset offset::operator-() const noexcept
{
	return offset{-m_offset};
}

constexpr offset& offset::operator++() noexcept
{
	++m_offset;
	return *this;
}

constexpr offset offset::operator++(int) noexcept
{
	offset temp{*this};
	++(*this);
	return temp;
}

constexpr offset& offset::operator--() noexcept
{
	--m_offset;
	return *this;
}

constexpr offset offset::operator--(int) noexcept
{
	offset temp{*this};
	--(*this);
	return temp;
}

constexpr offset& offset::operator+=(offset rhs) noexcept
{
	m_offset += rhs.m_offset;
	return *this;
}

constexpr offset& offset::operator-=(offset rhs) noexcept
{
	m_offset -= rhs.m_offset;
	return *this;
}

constexpr offset& offset::operator*=(streamoff rhs) noexcept
{
	m_offset *= rhs;
	return *this;
}

constexpr offset& offset::operator/=(streamoff rhs) noexcept
{
	m_offset /= rhs;
	return *this;
}

constexpr offset& offset::operator%=(streamoff rhs) noexcept
{
	m_offset %= rhs;
	return *this;
}

constexpr bool operator==(offset lhs, offset rhs) noexcept
{
	return lhs.value() == rhs.value();
}

constexpr strong_ordering operator<=>(offset lhs, offset rhs) noexcept
{
	return lhs.value() <=> rhs.value();
}

constexpr offset operator+(offset lhs, offset rhs) noexcept
{
	return lhs += rhs;
}

constexpr offset operator-(offset lhs, offset rhs) noexcept
{
	return lhs -= rhs;
}

constexpr offset operator*(offset lhs, streamoff rhs) noexcept
{
	return lhs *= rhs;
}

constexpr offset operator*(streamoff lhs, offset rhs) noexcept
{
	return rhs *= lhs;
}

constexpr offset operator/(offset lhs, streamoff rhs) noexcept
{
	return lhs /= rhs;
}

constexpr offset operator%(offset lhs, streamoff rhs) noexcept
{
	return lhs %= rhs;
}

}
