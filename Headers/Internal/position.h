/// \file
/// \brief Internal header file that describes the position class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "offset.h"

namespace std::io
{

class position final
{
public:
	// Constructors
	constexpr position() = default;
	constexpr explicit position(streamoff pos) noexcept;
	constexpr explicit position(offset off) noexcept;
	
	// Observer
	constexpr streamoff value() const noexcept;
	
	// Arithmetic
	constexpr position& operator++() noexcept;
	constexpr position operator++(int) noexcept;
	constexpr position& operator--() noexcept;
	constexpr position operator--(int) noexcept;
	
	constexpr position& operator+=(offset rhs) noexcept;
	constexpr position& operator-=(offset rhs) noexcept;
	
	// Special values
	static constexpr position max() noexcept;
private:
	streamoff m_position;
};

constexpr bool operator==(position lhs, position rhs) noexcept;
constexpr strong_ordering operator<=>(position lhs, position rhs) noexcept;

constexpr position operator+(position lhs, offset rhs) noexcept;
constexpr position operator+(offset lhs, position rhs) noexcept;
constexpr position operator-(position lhs, offset rhs) noexcept;
constexpr offset operator-(position lhs, position rhs) noexcept;

enum class base_position
{
	beginning,
	current,
	end
};

}

#include "position.hpp"
