/// \file
/// \brief Source file that contains implementation of the SpecialFile class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/special_file.h>

namespace std::io
{

SpecialFile::SpecialFile(native_handle_type handle)
	: BasicFile{handle}
{
}

streamsize SpecialFile::read_some(span<byte> buffer)
{
	return Platform::ReadSome(this->native_handle(), buffer);
}

streamsize SpecialFile::write_some(span<const byte> buffer)
{
	return Platform::WriteSome(this->native_handle(), buffer);
}

}
