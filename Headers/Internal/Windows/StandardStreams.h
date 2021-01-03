/// \file
/// \brief Internal header file that describes the Windows standard stream
/// objects.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "../any_input_stream.h"
#include "../any_output_stream.h"

#include "Platform.h"

namespace std::io::Windows
{

any_input_stream StandardInput() noexcept;

any_output_stream StandardOutput() noexcept;

any_output_stream StandardError() noexcept;

}
