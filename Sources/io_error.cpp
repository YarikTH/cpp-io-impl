/// \file
/// \brief Source file that contains implementation of the io_error class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/io_error.h>

namespace std::io
{

error_code make_error_code(io::io_errc e) noexcept
{
	return {static_cast<int>(e), io::category()};
}

error_condition make_error_condition(io::io_errc e) noexcept
{
	return {static_cast<int>(e), io::category()};
}

const char* category_impl::name() const noexcept
{
	return "io";
}

string category_impl::message(int ev) const
{
	auto value = static_cast<io_errc>(ev);
	switch (value)
	{
		case io_errc::bad_file_descriptor:
		{
			return "bad file descriptor";
		}
		case io_errc::invalid_argument:
		{
			return "invalid argument";
		}
		case io_errc::value_too_large:
		{
			return "value_too_large";
		}
		case io_errc::reached_end_of_file:
		{
			return "reached end of file";
		}
		case io_errc::interrupted:
		{
			return "interrupted";
		}
		case io_errc::physical_error:
		{
			return "psysical error";
		}
		case io_errc::file_too_large:
		{
			return "file too large";
		}
		default:
		{
			return "unknown error";
		}
	}
}

const error_category& category() noexcept
{
	static category_impl instance;
	return instance;
}

io_error::io_error(const string& message, error_code ec)
	: system_error{ec, message}
{
}

io_error::io_error(const char* message, error_code ec)
	: system_error{ec, message}
{
}

}
