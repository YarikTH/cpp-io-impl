/// \file
/// \brief Internal header file that describes the io_error class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <system_error>

namespace std
{
namespace io
{

enum class io_errc
{
	bad_file_descriptor = EBADF,
	invalid_argument = EINVAL,
	value_too_large = EOVERFLOW,
	reached_end_of_file,
	interrupted = EINTR,
	physical_error = EIO,
	file_too_large = EFBIG
};

}

template <>
struct is_error_code_enum<io::io_errc> : public true_type { };

namespace io
{

error_code make_error_code(io_errc e) noexcept;

error_condition make_error_condition(io_errc e) noexcept;

class category_impl final : public error_category
{
public:
	constexpr category_impl() noexcept = default;
	const char* name() const noexcept override;
	string message(int ev) const override;
};

const error_category& category() noexcept;

class io_error : public system_error
{
public:
	io_error(const string& message, error_code ec);
	io_error(const char* message, error_code ec);
};

}
}
