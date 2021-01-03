/// \file
/// \brief Source file that contains implementation of the POSIX standard stream
/// objects.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/POSIX/StandardStreams.h>

#include <unistd.h>
#include <sys/stat.h>

#include <Internal/input_file_stream.h>
#include <Internal/output_file_stream.h>
#include <Internal/synchronized_stream.h>
#include <Internal/special_file.h>

namespace std::io::POSIX
{

any_input_stream StandardInput() noexcept
{
	struct ::stat file_info;
	int result = ::fstat(STDIN_FILENO, &file_info);
	if (result == -1)
	{
		terminate();
	}
	if (S_ISREG(file_info.st_mode))
	{
		return SynchronizedStream{input_file_stream{STDIN_FILENO}};
	}
	if (S_ISCHR(file_info.st_mode))
	{
		return SynchronizedStream{SpecialFile{STDIN_FILENO}};
	}
	return SynchronizedStream{SpecialFile{STDIN_FILENO}};
}

any_output_stream StandardOutput() noexcept
{
	struct ::stat file_info;
	int result = ::fstat(STDOUT_FILENO, &file_info);
	if (result == -1)
	{
		terminate();
	}
	if (S_ISREG(file_info.st_mode))
	{
		return SynchronizedStream{output_file_stream{STDOUT_FILENO}};
	}
	if (S_ISCHR(file_info.st_mode))
	{
		return SynchronizedStream{SpecialFile{STDOUT_FILENO}};
	}
	return SynchronizedStream{SpecialFile{STDOUT_FILENO}};
}

any_output_stream StandardError() noexcept
{
	struct ::stat file_info;
	int result = ::fstat(STDERR_FILENO, &file_info);
	if (result == -1)
	{
		terminate();
	}
	if (S_ISREG(file_info.st_mode))
	{
		return SynchronizedStream{output_file_stream{STDERR_FILENO}};
	}
	if (S_ISCHR(file_info.st_mode))
	{
		return SynchronizedStream{SpecialFile{STDERR_FILENO}};
	}
	return SynchronizedStream{SpecialFile{STDERR_FILENO}};
}

}
