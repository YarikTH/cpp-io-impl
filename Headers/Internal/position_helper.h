/// \file
/// \brief Internal header file that describes the PositionHelper class
/// template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "position.h"

namespace std::io
{

/// \brief Helper class template for managing stream position.
/// \details This class template manages stream position and provides public
/// member functions that avoid code duplication.
/// \tparam Stream Type of the stream.
/// \tparam Position Type of the stream position.

template <typename Stream, typename Position>
class PositionHelper
{
public:
	// Constructor
	constexpr PositionHelper() noexcept;
	
	// Position
	constexpr position get_position() const noexcept;
	constexpr void seek_position(position pos);
	constexpr void seek_position(offset off);
	constexpr void seek_position(base_position base) noexcept;
	constexpr void seek_position(base_position base, offset off);
protected:
	Position m_position; ///< Stream position.
private:
	/// \brief Moves the stream position by the given offset.
	/// \param[in] pos Stream position to move.
	/// \param[in] off Offset to move by.
	/// \return Moved position.
	/// \throw std::io::io_error If resulting position is too large.
	static constexpr position MovePosition(Position pos, offset off);
};

}

#include "position_helper.hpp"
