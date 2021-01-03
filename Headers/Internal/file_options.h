/// \file
/// \brief Internal header file that describes the options used when opening
/// files.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

enum class mode
{
	read,
	write
};

enum class creation
{
	open_existing,
	if_needed,
	truncate_existing,
	always_new
};

}
