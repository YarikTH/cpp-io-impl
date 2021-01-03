/// \file
/// \brief Source file that contains implementation of the BasicFile class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/basic_file.h>

namespace std::io
{

BasicFile::BasicFile() noexcept
	: m_handle{Platform::InvalidNativeHandle}
{
}

BasicFile::BasicFile(native_handle_type handle)
	: m_handle{handle}
{
}

BasicFile::BasicFile(BasicFile&& other)
	: m_handle{other.m_handle}
{
	other.m_handle = Platform::InvalidNativeHandle;
}

BasicFile::~BasicFile()
{
	Platform::CloseFile(m_handle);
}

BasicFile& BasicFile::operator=(BasicFile&& other)
{
	BasicFile tmp{move(other)};
	ranges::swap(tmp.m_handle, m_handle);
	return *this;
}

auto BasicFile::native_handle() const noexcept -> native_handle_type
{
	return m_handle;
}

void BasicFile::assign(native_handle_type handle) noexcept
{
	Platform::CloseFile(m_handle);
	m_handle = handle;
}

auto BasicFile::release() noexcept -> native_handle_type
{
	auto result = m_handle;
	m_handle = Platform::InvalidNativeHandle;
	return result;
}

}
