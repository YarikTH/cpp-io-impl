/// \file
/// \brief Internal header file that describes the input_file_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "file_stream_base.h"

namespace std::io
{

class input_file_stream final : public file_stream_base
{
public:
	// Construct/copy/destroy
	input_file_stream() noexcept = default;
	input_file_stream(const filesystem::path& file_name);
	input_file_stream(native_handle_type handle);
	
	// Reading
	streamsize read_some(span<byte> buffer);
};

}
