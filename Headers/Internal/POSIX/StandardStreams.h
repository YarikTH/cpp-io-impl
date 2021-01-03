/// \file
/// \brief Internal header file that describes the POSIX standard stream
/// objects.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "../any_input_stream.h"
#include "../any_output_stream.h"

#include "Platform.h"

namespace std::io::POSIX
{

/// \brief Returns the standard input stream.
/// \return Standard input stream.
any_input_stream StandardInput() noexcept;

/// \brief Returns the standard output stream.
/// \return Standard output stream.
any_output_stream StandardOutput() noexcept;

/// \brief Returns the standard error stream.
/// \return Standard error stream.
any_output_stream StandardError() noexcept;

}
