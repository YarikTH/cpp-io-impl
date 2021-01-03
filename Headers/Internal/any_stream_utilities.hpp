/// \file
/// \brief Internal header file that contains implementation of the utility
/// classes and functions for type erased streams.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "io_error.h"

namespace std::io::Any
{

template <typename B, stream S>
constexpr SharedImpl<B, S>::SharedImpl(const S& s)
requires copyable<S>
	: m_stream{s}
{
}

template <typename B, stream S>
constexpr SharedImpl<B, S>::SharedImpl(S&& s)
requires movable<S>
	: m_stream{move(s)}
{
}

template <typename B, stream S>
template <typename... Args>
requires constructible_from<S, Args...>
constexpr SharedImpl<B, S>::SharedImpl(in_place_t, Args&&... args)
	: m_stream{forward<Args>(args)...}
{
}

template <typename B, stream S>
constexpr const type_info& SharedImpl<B, S>::type() const noexcept
{
	return typeid(S);
}

template <typename B, stream S>
constexpr const S& SharedImpl<B, S>::get() const noexcept
{
	return m_stream;
}

template <typename B, stream S>
constexpr S& SharedImpl<B, S>::get() noexcept
{
	return m_stream;
}

template <typename B, stream S>
constexpr S SharedImpl<B, S>::release() noexcept
{
	return move(m_stream);
}

template <typename B, stream S>
constexpr position SharedImpl<B, S>::get_position() const
{
	if constexpr (!seekable_stream<S>)
	{
		throw io_error{"get_position: Stream is not seekable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		return m_stream.get_position();
	}
}

template <typename B, stream S>
constexpr void SharedImpl<B, S>::seek_position(position pos)
{
	if constexpr (!seekable_stream<S>)
	{
		throw io_error{"seek_position: Stream is not seekable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		m_stream.seek_position(pos);
	}
}

template <typename B, stream S>
constexpr void SharedImpl<B, S>::seek_position(offset off)
{
	if constexpr (!seekable_stream<S>)
	{
		throw io_error{"seek_position: Stream is not seekable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		m_stream.seek_position(off);
	}
}

template <typename B, stream S>
constexpr void SharedImpl<B, S>::seek_position(base_position base)
{
	if constexpr (!seekable_stream<S>)
	{
		throw io_error{"seek_position: Stream is not seekable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		m_stream.seek_position(base);
	}
}

template <typename B, stream S>
constexpr void SharedImpl<B, S>::seek_position(base_position base, offset off)
{
	if constexpr (!seekable_stream<S>)
	{
		throw io_error{"seek_position: Stream is not seekable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		m_stream.seek_position(base, off);
	}
}

template <typename B, stream S>
constexpr void SharedImpl<B, S>::flush()
{
	if constexpr (!buffered_stream<S>)
	{
		return;
	}
	else
	{
		m_stream.flush();
	}
}

constexpr StreamBase::StreamBase() noexcept
	: m_impl{nullptr}
{
}

constexpr StreamBase::StreamBase(SharedBase* impl)
	: m_impl{impl}
{
}

constexpr StreamBase::StreamBase(const StreamBase& other)
{
	if (other.m_impl == nullptr)
	{
		m_impl = nullptr;
		return;
	}
	m_impl = other.m_impl->Copy();
}

constexpr StreamBase::StreamBase(StreamBase&& other) noexcept
	: m_impl{other.m_impl}
{
	other.m_impl = nullptr;
}

constexpr StreamBase::~StreamBase()
{
	this->reset();
}

constexpr StreamBase& StreamBase::operator=(const StreamBase& other)
{
	StreamBase(other).swap(*this);
	return *this;
}

constexpr StreamBase& StreamBase::operator=(StreamBase&& other) noexcept
{
	StreamBase(move(other)).swap(*this);
	return *this;
}

constexpr bool StreamBase::has_value() const noexcept
{
	return m_impl != nullptr;
}

constexpr const type_info& StreamBase::type() const noexcept
{
	if (!this->has_value())
	{
		return typeid(void);
	}
	return m_impl->type();
}

constexpr void StreamBase::reset() noexcept
{
	delete m_impl;
	m_impl = nullptr;
}

constexpr void StreamBase::swap(StreamBase& other) noexcept
{
	ranges::swap(m_impl, other.m_impl);
}

constexpr position StreamBase::get_position() const
{
	CheckStream(m_impl);
	return m_impl->get_position();
}

constexpr void StreamBase::seek_position(position pos)
{
	CheckStream(m_impl);
	m_impl->seek_position(pos);
}

constexpr void StreamBase::seek_position(offset off)
{
	CheckStream(m_impl);
	m_impl->seek_position(off);
}

constexpr void StreamBase::seek_position(base_position base)
{
	CheckStream(m_impl);
	m_impl->seek_position(base);
}

constexpr void StreamBase::seek_position(base_position base, offset off)
{
	CheckStream(m_impl);
	m_impl->seek_position(base, off);
}

constexpr void StreamBase::flush()
{
	CheckStream(m_impl);
	m_impl->flush();
}

constexpr void CheckStream(auto& ptr)
{
	if (ptr == nullptr)
	{
		throw io_error{"CheckStream: Erased stream is empty",
			io_errc::bad_file_descriptor};
	}
}

}
