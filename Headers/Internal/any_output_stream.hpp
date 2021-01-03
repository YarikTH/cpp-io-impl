/// \file
/// \brief Internal header file that contains implementation of the
/// any_output_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

template <output_stream S>
constexpr any_output_stream::any_output_stream(S&& s)
	: Any::StreamBase{new Impl<decay_t<S>>{forward<S>(s)}}
{
}

template <output_stream S, typename... Args>
requires constructible_from<S, Args...>
constexpr any_output_stream::any_output_stream(in_place_type_t<S>,
	Args&&... args)
	: Any::StreamBase{new Impl<decay_t<S>>{in_place, forward<Args>(args)...}}
{
}

template <output_stream S>
constexpr any_output_stream& any_output_stream::operator=(S&& s)
{
	any_output_stream(forward<S>(s)).swap(*this);
	return *this;
}

template <output_stream S>
constexpr const S& any_output_stream::get() const
{
	auto true_impl = dynamic_cast<const Impl<decay_t<S>>*>(m_impl);
	if (true_impl == nullptr)
	{
		throw bad_any_cast{};
	}
	return true_impl->get();
}

template <output_stream S>
constexpr S& any_output_stream::get()
{
	auto true_impl = dynamic_cast<Impl<decay_t<S>>*>(m_impl);
	if (true_impl == nullptr)
	{
		throw bad_any_cast{};
	}
	return true_impl->get();
}

template <output_stream S, typename... Args>
requires constructible_from<S, Args...>
constexpr void any_output_stream::emplace(Args&&... args)
{
	this->reset();
	m_impl = new Impl<decay_t<S>>{forward<Args>(args)...};
}

template <output_stream S>
requires movable<S>
constexpr S any_output_stream::release()
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

constexpr void any_output_stream::swap(any_output_stream& other) noexcept
{
	Any::StreamBase::swap(other);
}

constexpr streamsize any_output_stream::write_some(span<const byte> buffer)
{
	Any::CheckStream(m_impl);
	return static_cast<Base*>(m_impl)->write_some(buffer);
}

template <output_stream S>
constexpr any_output_stream::Impl<S>::Impl(const S& s)
requires copyable<S>
	: Any::SharedImpl<Base, S>{s}
{
}

template <output_stream S>
constexpr any_output_stream::Impl<S>::Impl(S&& s)
requires movable<S>
	: Any::SharedImpl<Base, S>{move(s)}
{
}

template <output_stream S>
template <typename... Args>
requires constructible_from<S, Args...>
constexpr any_output_stream::Impl<S>::Impl(in_place_t, Args&&... args)
	: Any::SharedImpl<Base, S>{in_place, forward<Args>(args)...}
{
}

template <output_stream S>
constexpr Any::SharedBase* any_output_stream::Impl<S>::Copy() const
{
	if constexpr (!copyable<S>)
	{
		throw io_error{"any_output_stream::Impl::Copy: Stream is not copyable",
			io_errc::bad_file_descriptor};
	}
	else
	{
		return new Impl<S>{this->m_stream};
	}
}

template <output_stream S>
constexpr streamsize any_output_stream::Impl<S>::write_some(
	span<const byte> buffer)
{
	return this->m_stream.write_some(buffer);
}

}
