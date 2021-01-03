/// \file
/// \brief Source file that contains implementation of the File class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/file.h>

namespace std::io
{

File::File(const filesystem::path& file_name, mode m, creation c)
	: BasicFile{Platform::OpenFile(file_name, m, c)}
{
}

File::File(native_handle_type handle)
	: BasicFile{handle}
{
}

position File::get_position() const
{
	return Platform::GetPosition(this->native_handle());
}

void File::seek_position(position pos)
{
	Platform::SeekPosition(this->native_handle(), pos);
}

void File::seek_position(offset off)
{
	Platform::SeekPosition(this->native_handle(), off);
}

void File::seek_position(base_position base)
{
	Platform::SeekPosition(this->native_handle(), base);
}

void File::seek_position(base_position base, offset off)
{
	Platform::SeekPosition(this->native_handle(), base, off);
}

streamsize File::read_some(span<byte> buffer)
{
	return Platform::ReadSome(this->native_handle(), buffer);
}

streamsize File::write_some(span<const byte> buffer)
{
	return Platform::WriteSome(this->native_handle(), buffer);
}

}
