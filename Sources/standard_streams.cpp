/// \file
/// \brief Internal header file that contains implementation of the standard
/// stream objects.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/standard_streams.h>

#include "StandardStreams.h"

namespace std::io
{

any_input_stream& in() noexcept
{
	static any_input_stream in_stream = Platform::StandardInput();
	return in_stream;
}

any_output_stream& out() noexcept
{
	static any_output_stream out_stream = Platform::StandardOutput();
	return out_stream;
}

any_output_stream& err() noexcept
{
	static any_output_stream err_stream = Platform::StandardError();
	return err_stream;
}

}
