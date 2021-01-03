/// \file
/// \brief Source file that contains implementation of the Windows standard
/// stream objects.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/Windows/StandardStreams.h>

#include <Internal/input_file_stream.h>
#include <Internal/output_file_stream.h>
//#include <Internal/synchronized_stream.h>
#include <Internal/special_file.h>

namespace std::io::Windows
{

any_input_stream StandardInput() noexcept
{
	auto handle = ::GetStdHandle(STD_INPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE)
	{
		terminate();
	}
	switch (::GetFileType(handle))
	{
		case FILE_TYPE_DISK:
		{
			// TODO: Find a way to compile with <mutex>
			//return SynchronizedStream{input_file_stream{handle}};
			return input_file_stream{handle};
		}
		case FILE_TYPE_CHAR:
		case FILE_TYPE_PIPE:
		{
			// TODO: Find a way to compile with <mutex>
			//return SynchronizedStream{SpecialFile{handle}};
			return SpecialFile{handle};
		}
		case FILE_TYPE_UNKNOWN:
		default:
		{
			terminate();
		}
	}
}

any_output_stream StandardOutput() noexcept
{
	auto handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE)
	{
		terminate();
	}
	switch (::GetFileType(handle))
	{
		case FILE_TYPE_DISK:
		{
			// TODO: Find a way to compile with <mutex>
			//return SynchronizedStream{output_file_stream{handle}};
			return output_file_stream{handle};
		}
		case FILE_TYPE_CHAR:
		case FILE_TYPE_PIPE:
		{
			// TODO: Find a way to compile with <mutex>
			//return SynchronizedStream{SpecialFile{handle}};
			return SpecialFile{handle};
		}
		case FILE_TYPE_UNKNOWN:
		default:
		{
			terminate();
		}
	}
}

any_output_stream StandardError() noexcept
{
	auto handle = ::GetStdHandle(STD_ERROR_HANDLE);
	if (handle == INVALID_HANDLE_VALUE)
	{
		terminate();
	}
	switch (::GetFileType(handle))
	{
		case FILE_TYPE_DISK:
		{
			// TODO: Find a way to compile with <mutex>
			//return SynchronizedStream{output_file_stream{handle}};
			return output_file_stream{handle};
		}
		case FILE_TYPE_CHAR:
		case FILE_TYPE_PIPE:
		{
			// TODO: Find a way to compile with <mutex>
			//return SynchronizedStream{SpecialFile{handle}};
			return SpecialFile{handle};
		}
		case FILE_TYPE_UNKNOWN:
		default:
		{
			terminate();
		}
	}
}

}
