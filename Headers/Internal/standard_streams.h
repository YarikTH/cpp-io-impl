/// \file
/// \brief Internal header file that describes the standard stream objects.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "any_input_stream.h"
#include "any_output_stream.h"

namespace std::io
{

any_input_stream& in() noexcept;

any_output_stream& out() noexcept;

any_output_stream& err() noexcept;

}
