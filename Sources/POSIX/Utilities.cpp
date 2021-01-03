/// \file
/// \brief Source file that contains implementation of the utility functions for
/// POSIX IO.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/POSIX/Utilities.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/statvfs.h>

#include <Internal/io_error.h>

namespace std::io::POSIX
{

NativeHandle OpenFile(const filesystem::path& file_name, mode m, creation c)
{
	int raw_mode = 0;
	switch (m)
	{
		case mode::read:
		{
			raw_mode = O_RDONLY;
			break;
		}
		case mode::write:
		{
			raw_mode = O_RDWR | O_CREAT;
			break;
		}
	}
	switch (c)
	{
		case creation::open_existing:
		{
			break;
		}
		case creation::if_needed:
		{
			raw_mode |= O_CREAT;
			break;
		}
		case creation::truncate_existing:
		{
			raw_mode |= O_TRUNC;
			break;
		}
		case creation::always_new:
		{
			raw_mode |= O_CREAT;
			if (filesystem::exists(file_name))
			{
				filesystem::remove(file_name);
			}
			break;
		}
	}
	auto handle = ::open(file_name.c_str(), raw_mode,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if (handle != -1)
	{
		return handle;
	}
	// TODO: Better error handling.
	throw system_error{errno, generic_category(), "OpenFile: open() failed"};
}

void CloseFile(NativeHandle handle) noexcept
{
	if (handle < 0)
	{
		return;
	}
	::close(handle);
}

position GetPosition(NativeHandle handle)
{
	off_t result = ::lseek(handle, 0, SEEK_CUR);
	if (result != -1)
	{
		return position{result};
	}
	const char* message = "GetPosition: lseek() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, position pos)
{
	off_t result = ::lseek(handle, pos.value(), SEEK_SET);
	if (result != -1)
	{
		return;
	}
	const char* message = "SeekPosition: lseek() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		case EINVAL:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		case EOVERFLOW:
		{
			throw io_error{message, io_errc::value_too_large};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, offset off)
{
	off_t result = ::lseek(handle, off.value(), SEEK_CUR);
	if (result != -1)
	{
		return;
	}
	const char* message = "SeekPosition: lseek() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		case EINVAL:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		case EOVERFLOW:
		{
			throw io_error{message, io_errc::value_too_large};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, base_position base)
{
	int raw_base;
	switch (base)
	{
		case base_position::beginning:
		{
			raw_base = SEEK_SET;
			break;
		}
		case base_position::current:
		{
			return;
		}
		case base_position::end:
		{
			raw_base = SEEK_END;
			break;
		}
	}
	off_t result = ::lseek(handle, 0, raw_base);
	if (result != -1)
	{
		return;
	}
	const char* message = "SeekPosition: lseek() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		case EINVAL:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		case EOVERFLOW:
		{
			throw io_error{message, io_errc::value_too_large};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, base_position base, offset off)
{
	int raw_base;
	switch (base)
	{
		case base_position::beginning:
		{
			raw_base = SEEK_SET;
			break;
		}
		case base_position::current:
		{
			raw_base = SEEK_CUR;
			break;
		}
		case base_position::end:
		{
			raw_base = SEEK_END;
			break;
		}
	}
	off_t result = ::lseek(handle, off.value(), raw_base);
	if (result != -1)
	{
		return;
	}
	const char* message = "SeekPosition: lseek() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		case EINVAL:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		case EOVERFLOW:
		{
			throw io_error{message, io_errc::value_too_large};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

streamsize ReadSome(NativeHandle handle, span<byte> buffer)
{
	ptrdiff_t bytes_to_read = ranges::ssize(buffer);
	if (bytes_to_read == 0)
	{
		return 0;
	}
	ssize_t result = ::read(handle, ranges::data(buffer), bytes_to_read);
	if (result != -1)
	{
		return result;
	}
	const char* message = "ReadSome: read() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		case EINTR:
		{
			throw io_error{message, io_errc::interrupted};
		}
		case EIO:
		{
			throw io_error{message, io_errc::physical_error};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

streamsize WriteSome(NativeHandle handle, span<const byte> buffer)
{
	ptrdiff_t bytes_to_write = ranges::ssize(buffer);
	if (bytes_to_write == 0)
	{
		return 0;
	}
	ssize_t result = ::write(handle, ranges::data(buffer), bytes_to_write);
	if (result != -1)
	{
		return result;
	}
	const char* message = "WriteSome: write() failed";
	switch (errno)
	{
		case EBADF:
		{
			throw io_error{message, io_errc::bad_file_descriptor};
		}
		case EFBIG:
		{
			throw io_error{message, io_errc::file_too_large};
		}
		case EINTR:
		{
			throw io_error{message, io_errc::interrupted};
		}
		case EIO:
		{
			throw io_error{message, io_errc::physical_error};
		}
		default:
		{
			// TODO: Better error handling.
			throw system_error{errno, generic_category(), message};
		}
	}
}

size_t GetBufferSize(NativeHandle handle)
{
	struct ::statvfs stats;
	int result = ::fstatvfs(handle, &stats);
	if (result == -1)
	{
		// TODO: Better error handling.
		throw system_error{errno, generic_category(),
			"GetBufferSize: fstatvfs() failed"};
	}
	return stats.f_bsize;
}

}
