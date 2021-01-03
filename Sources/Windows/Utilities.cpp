/// \file
/// \brief Source file that contains implementation of the utility functions for
/// Windows IO.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/Windows/Utilities.h>

#include <Internal/io_error.h>

namespace std::io::Windows
{

NativeHandle OpenFile(const filesystem::path& file_name, mode m, creation c)
{
	DWORD access = 0;
	switch (m)
	{
		case mode::read:
		{
			access |= GENERIC_READ;
			break;
		}
		case mode::write:
		{
			access |= GENERIC_READ | GENERIC_WRITE;
			break;
		}
	}
	DWORD creation_value = 0;
	switch (c)
	{
		case creation::open_existing:
		{
			creation_value = OPEN_EXISTING;
			break;
		}
		case creation::if_needed:
		{
			creation_value = OPEN_ALWAYS;
			break;
		}
		case creation::truncate_existing:
		{
			creation_value = TRUNCATE_EXISTING;
			break;
		}
		case creation::always_new:
		{
			creation_value = CREATE_ALWAYS;
			break;
		}
	}
	auto handle = ::CreateFileW(file_name.c_str(), access, 0, nullptr,
		creation_value, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle != INVALID_HANDLE_VALUE)
	{
		return handle;
	}
	// TODO: Better error handling.
	throw system_error{static_cast<int>(::GetLastError()), system_category(),
		"OpenFile: CreateFileW() failed"};
}

void CloseFile(NativeHandle handle) noexcept
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		return;
	}
	::CloseHandle(handle);
}

position GetPosition(NativeHandle handle)
{
	LARGE_INTEGER pos{ .QuadPart = 0 };
	LARGE_INTEGER new_position;
	BOOL result = ::SetFilePointerEx(handle, pos, &new_position, FILE_CURRENT);
	if (result != 0)
	{
		return position{new_position.QuadPart};
	}
	throw system_error{static_cast<int>(::GetLastError()), system_category(),
		"GetPosition: SetFilePointerEx() failed"};
}

void SeekPosition(NativeHandle handle, position pos)
{
	LARGE_INTEGER raw_position{ .QuadPart = pos.value() };
	BOOL result = ::SetFilePointerEx(handle, raw_position, nullptr, FILE_BEGIN);
	if (result != 0)
	{
		return;
	}
	const char* message = "SeekPosition: SetFilePointerEx() failed";
	switch (::GetLastError())
	{
		case ERROR_NEGATIVE_SEEK:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		default:
		{
			throw system_error{static_cast<int>(::GetLastError()),
				system_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, offset off)
{
	LARGE_INTEGER raw_position{ .QuadPart = off.value() };
	BOOL result = ::SetFilePointerEx(handle, raw_position, nullptr,
		FILE_CURRENT);
	if (result != 0)
	{
		return;
	}
	const char* message = "SeekPosition: SetFilePointerEx() failed";
	switch (::GetLastError())
	{
		case ERROR_NEGATIVE_SEEK:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		default:
		{
			throw system_error{static_cast<int>(::GetLastError()),
				system_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, base_position base)
{
	LARGE_INTEGER raw_offset{ .QuadPart = 0 };
	DWORD raw_base;
	switch (base)
	{
		case base_position::beginning:
		{
			raw_base = FILE_BEGIN;
			break;
		}
		case base_position::current:
		{
			return;
		}
		case base_position::end:
		{
			raw_base = FILE_END;
			break;
		}
	}
	BOOL result = ::SetFilePointerEx(handle, raw_offset, nullptr, raw_base);
	if (result != 0)
	{
		return;
	}
	const char* message = "SeekPosition: SetFilePointerEx() failed";
	switch (::GetLastError())
	{
		case ERROR_NEGATIVE_SEEK:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		default:
		{
			throw system_error{static_cast<int>(::GetLastError()),
				system_category(), message};
		}
	}
}

void SeekPosition(NativeHandle handle, base_position base, offset off)
{
	LARGE_INTEGER raw_offset{ .QuadPart = off.value() };
	DWORD raw_base;
	switch (base)
	{
		case base_position::beginning:
		{
			raw_base = FILE_BEGIN;
			break;
		}
		case base_position::current:
		{
			raw_base = FILE_CURRENT;
			break;
		}
		case base_position::end:
		{
			raw_base = FILE_END;
			break;
		}
	}
	BOOL result = ::SetFilePointerEx(handle, raw_offset, nullptr, raw_base);
	if (result != 0)
	{
		return;
	}
	const char* message = "SeekPosition: SetFilePointerEx() failed";
	switch (::GetLastError())
	{
		case ERROR_NEGATIVE_SEEK:
		{
			throw io_error{message, io_errc::invalid_argument};
		}
		default:
		{
			throw system_error{static_cast<int>(::GetLastError()),
				system_category(), message};
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
	DWORD bytes_read;
	BOOL result = ::ReadFile(handle, ranges::data(buffer), bytes_to_read,
		&bytes_read, nullptr);
	if (result != FALSE)
	{
		return bytes_read;
	}
	throw system_error{static_cast<int>(::GetLastError()), system_category(),
		"ReadSome: ReadFile() failed"};
}

streamsize WriteSome(NativeHandle handle, span<const byte> buffer)
{
	ptrdiff_t bytes_to_write = ranges::ssize(buffer);
	if (bytes_to_write == 0)
	{
		return 0;
	}
	DWORD bytes_written;
	BOOL result = ::WriteFile(handle, ranges::data(buffer), bytes_to_write,
		&bytes_written, nullptr);
	if (result != FALSE)
	{
		return bytes_written;
	}
	throw system_error{static_cast<int>(::GetLastError()), system_category(),
		"WriteSome: WriteFile() failed"};
}

size_t GetBufferSize(NativeHandle handle)
{
	// TODO: Try to actually get sector size.
	return 4096;
}

}
