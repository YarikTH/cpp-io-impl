/// \file
/// \brief Internal header file that describes the offset class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <ios>

namespace std::io
{

class offset final
{
public:
	// Constructors
	constexpr offset() = default;
	constexpr explicit offset(streamoff off) noexcept;
	
	// Observer
	constexpr streamoff value() const noexcept;
	
	// Arithmetic
	constexpr offset operator+() const noexcept;
	constexpr offset operator-() const noexcept;
	constexpr offset& operator++() noexcept;
	constexpr offset operator++(int) noexcept;
	constexpr offset& operator--() noexcept;
	constexpr offset operator--(int) noexcept;
	
	constexpr offset& operator+=(offset rhs) noexcept;
	constexpr offset& operator-=(offset rhs) noexcept;
	
	constexpr offset& operator*=(streamoff rhs) noexcept;
	constexpr offset& operator/=(streamoff rhs) noexcept;
	constexpr offset& operator%=(streamoff rhs) noexcept;
private:
	streamoff m_offset;
};

constexpr bool operator==(offset lhs, offset rhs) noexcept;
constexpr strong_ordering operator<=>(offset lhs, offset rhs) noexcept;

constexpr offset operator+(offset lhs, offset rhs) noexcept;
constexpr offset operator-(offset lhs, offset rhs) noexcept;
constexpr offset operator*(offset lhs, streamoff rhs) noexcept;
constexpr offset operator*(streamoff lhs, offset rhs) noexcept;
constexpr offset operator/(offset lhs, streamoff rhs) noexcept;
constexpr offset operator%(offset lhs, streamoff rhs) noexcept;

}

#include "offset.hpp"
