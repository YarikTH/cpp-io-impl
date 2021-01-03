/// \file
/// \brief Internal header file that describes the SpecialFile class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "basic_file.h"

namespace std::io
{

/// \brief A special file.
/// \details This class provides the fallback that does absolute minimum -
/// reading and writing. It is only used as implementation detail for standard
/// streams where those streams are not regular files.

class SpecialFile final : public BasicFile
{
public:
	using BasicFile::native_handle_type;
	
	// Constructor
	SpecialFile(native_handle_type handle);
	
	// Reading
	streamsize read_some(span<byte> buffer);
	
	// Writing
	streamsize write_some(span<const byte> buffer);
};

}
