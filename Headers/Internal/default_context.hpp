/// \file
/// \brief Internal header file that contains implementation of the
/// default_context class template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

template <stream S>
constexpr default_context<S>::default_context(S& s, format f) noexcept
	: m_stream{s},
	m_format{f}
{
}

template <stream S>
constexpr S& default_context<S>::get_stream() noexcept
{
	return m_stream;
}

template <stream S>
constexpr const S& default_context<S>::get_stream() const noexcept
{
	return m_stream;
}

template <stream S>
constexpr format default_context<S>::get_format() const noexcept
{
	return m_format;
}

template <stream S>
constexpr void default_context<S>::set_format(format f) noexcept
{
	m_format = f;
}

}
