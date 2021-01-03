/// \file
/// \brief Internal header file that contains implementation of the position
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

constexpr position::position(streamoff off) noexcept
	: m_position{off}
{
}

constexpr position::position(offset off) noexcept
	: m_position{off.value()}
{
}

constexpr streamoff position::value() const noexcept
{
	return m_position;
}

constexpr position& position::operator++() noexcept
{
	++m_position;
	return *this;
}

constexpr position position::operator++(int) noexcept
{
	position temp{*this};
	++(*this);
	return temp;
}

constexpr position& position::operator--() noexcept
{
	--m_position;
	return *this;
}

constexpr position position::operator--(int) noexcept
{
	position temp{*this};
	--(*this);
	return temp;
}

constexpr position& position::operator+=(offset rhs) noexcept
{
	m_position += rhs.value();
	return *this;
}

constexpr position& position::operator-=(offset rhs) noexcept
{
	m_position -= rhs.value();
	return *this;
}

constexpr position position::max() noexcept
{
	return position{numeric_limits<streamoff>::max()};
}

constexpr bool operator==(position lhs, position rhs) noexcept
{
	return lhs.value() == rhs.value();
}

constexpr strong_ordering operator<=>(position lhs, position rhs) noexcept
{
	return lhs.value() <=> rhs.value();
}

constexpr position operator+(position lhs, offset rhs) noexcept
{
	return lhs += rhs;
}

constexpr position operator+(offset lhs, position rhs) noexcept
{
	return rhs += lhs;
}

constexpr position operator-(position lhs, offset rhs) noexcept
{
	return lhs -= rhs;
}

constexpr offset operator-(position lhs, position rhs) noexcept
{
	return offset{lhs.value() - rhs.value()};
}

}
