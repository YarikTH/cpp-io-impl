/// \file
/// \brief Internal header file that contains implementation of the
/// any_input_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <any>

namespace std::io
{

template <input_stream S>
constexpr any_input_stream::any_input_stream(S&& s)
	: Any::StreamBase{new Impl<decay_t<S>>{forward<S>(s)}}
{
}

template <input_stream S, typename... Args>
requires constructible_from<S, Args...>
constexpr any_input_stream::any_input_stream(in_place_type_t<S>, Args&&... args)
	: Any::StreamBase{new Impl<decay_t<S>>{in_place, forward<Args>(args)...}}
{
}

template <input_stream S>
constexpr any_input_stream& any_input_stream::operator=(S&& s)
{
	any_input_stream(forward<S>(s)).swap(*this);
	return *this;
}

template <input_stream S>
constexpr const S& any_input_stream::get() const
{
	auto true_impl = dynamic_cast<const Impl<decay_t<S>>*>(m_impl);
	if (true_impl == nullptr)
	{
		throw bad_any_cast{};
	}
	return true_impl->get();
}

template <input_stream S>
constexpr S& any_input_stream::get()
{
	auto true_impl = dynamic_cast<Impl<decay_t<S>>*>(m_impl);
	if (true_impl == nullptr)
	{
		throw bad_any_cast{};
	}
	return true_impl->get();
}

template <input_stream S, typename... Args>
requires constructible_from<S, Args...>
constexpr void any_input_stream::emplace(Args&&... args)
{
	this->reset();
	m_impl = new Impl<decay_t<S>>{forward<Args>(args)...};
}

template <input_stream S>
requires movable<S>
constexpr S any_input_stream::release()
{
	auto true_impl = dynamic_cast<Impl<decay_t<S>>*>(m_impl);
	if (true_impl == nullptr)
	{
		throw bad_any_cast{};
	}
	S result = true_impl.release();
	this->reset();
	return result;
}

constexpr void any_input_stream::swap(any_input_stream& other) noexcept
{
	Any::StreamBase::swap(other);
}

constexpr streamsize any_input_stream::read_some(span<byte> buffer)
{
	Any::CheckStream(m_impl);
	return static_cast<Base*>(m_impl)->read_some(buffer);
}

template <input_stream S>
constexpr any_input_stream::Impl<S>::Impl(const S& s)
requires copyable<S>
	: Any::SharedImpl<Base, S>{s}
{
}

template <input_stream S>
constexpr any_input_stream::Impl<S>::Impl(S&& s)
requires movable<S>
	: Any::SharedImpl<Base, S>{move(s)}
{
}

template <input_stream S>
template <typename... Args>
requires constructible_from<S, Args...>
constexpr any_input_stream::Impl<S>::Impl(in_place_t, Args&&... args)
	: Any::SharedImpl<Base, S>{in_place, forward<Args>(args)...}
{
}

template <input_stream S>
constexpr Any::SharedBase* any_input_stream::Impl<S>::Copy() const
{
	if constexpr (!copyable<S>)
	{
		throw io_error{"any_input_stream::Impl::Copy: Stream is not copyable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		return new Impl<S>{this->m_stream};
	}
}

template <input_stream S>
constexpr streamsize any_input_stream::Impl<S>::read_some(span<byte> buffer)
{
	return this->m_stream.read_some(buffer);
}

}
