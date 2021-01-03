/// \file
/// \brief Internal header file that contains implementation of the
/// SynchronizedStream class template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

template <stream S>
SynchronizedStream<S>::SynchronizedStream(const S& s)
requires copyable<S>
	: m_stream{s}
{
}

template <stream S>
SynchronizedStream<S>::SynchronizedStream(S&& s)
requires movable<S>
	: m_stream{move(s)}
{
}

template <stream S>
SynchronizedStream<S>::SynchronizedStream(const SynchronizedStream& other)
requires copyable<S>
	: m_stream{other.m_stream},
	m_mutex{}
{
}

template <stream S>
SynchronizedStream<S>::SynchronizedStream(SynchronizedStream&& other)
requires movable<S>
	: m_stream{move(other.m_stream)},
	m_mutex{}
{
}

template <stream S>
auto SynchronizedStream<S>::operator=(const SynchronizedStream& other) ->
	SynchronizedStream&
requires copyable<S>
{
	lock_guard guard1{m_mutex};
	lock_guard guard2{other.m_mutex};
	m_stream = other.m_stream;
	return *this;
}

template <stream S>
auto SynchronizedStream<S>::operator=(SynchronizedStream&& other) ->
	SynchronizedStream&
requires movable<S>
{
	lock_guard guard1{m_mutex};
	lock_guard guard2{other.m_mutex};
	m_stream = move(other.m_stream);
	return *this;
}

template <stream S>
position SynchronizedStream<S>::get_position() const
requires seekable_stream<S>
{
	lock_guard guard{m_mutex};
	return m_stream.get_position();
}

template <stream S>
void SynchronizedStream<S>::seek_position(position pos)
requires seekable_stream<S>
{
	lock_guard guard{m_mutex};
	m_stream.seek_position(pos);
}

template <stream S>
void SynchronizedStream<S>::seek_position(offset off)
requires seekable_stream<S>
{
	lock_guard guard{m_mutex};
	m_stream.seek_position(off);
}

template <stream S>
void SynchronizedStream<S>::seek_position(base_position base)
requires seekable_stream<S>
{
	lock_guard guard{m_mutex};
	m_stream.seek_position(base);
}

template <stream S>
void SynchronizedStream<S>::seek_position(base_position base, offset off)
requires seekable_stream<S>
{
	lock_guard guard{m_mutex};
	m_stream.seek_position(base, off);
}

template <stream S>
void SynchronizedStream<S>::flush()
requires buffered_stream<S>
{
	lock_guard guard{m_mutex};
	m_stream.flush();
}

template <stream S>
streamsize SynchronizedStream<S>::read_some(span<byte> buffer)
requires input_stream<S>
{
	lock_guard guard{m_mutex};
	return m_stream.read_some(buffer);
}

template <stream S>
streamsize SynchronizedStream<S>::write_some(span<const byte> buffer)
requires output_stream<S>
{
	lock_guard guard{m_mutex};
	return m_stream.write_some(buffer);
}

}
