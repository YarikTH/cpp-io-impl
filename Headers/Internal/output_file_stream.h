/// \file
/// \brief Internal header file that describes the output_file_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "file_stream_base.h"

namespace std::io
{

class output_file_stream final : public file_stream_base
{
public:
	// Construct/copy/destroy
	output_file_stream() noexcept = default;
	output_file_stream(const filesystem::path& file_name,
		creation c = creation::if_needed);
	output_file_stream(native_handle_type handle);
	
	// Writing
	streamsize write_some(span<const byte> buffer);
};

}
