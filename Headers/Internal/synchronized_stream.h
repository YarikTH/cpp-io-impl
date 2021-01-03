/// \file
/// \brief Internal header file that describes the SynchronizedStream class
/// template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include <mutex>

namespace std::io
{

/// \brief Synchronized stream.
/// \details This class template implements synchronization required by default
/// contained streams of standard stream objects.
/// \tparam S Type of the stream to synchronize access to.

template <stream S>
class SynchronizedStream final
{
public:
	// Constructors
	SynchronizedStream(const S& s)
	requires copyable<S>;
	
	SynchronizedStream(S&& s)
	requires movable<S>;
	
	SynchronizedStream(const SynchronizedStream& other)
	requires copyable<S>;
	
	SynchronizedStream(SynchronizedStream&& other)
	requires movable<S>;
	
	// Assignment
	SynchronizedStream& operator=(const SynchronizedStream& other)
	requires copyable<S>;
	
	SynchronizedStream& operator=(SynchronizedStream&& other)
	requires movable<S>;
	
	// Position
	position get_position() const
	requires seekable_stream<S>;
	void seek_position(position pos)
	requires seekable_stream<S>;
	void seek_position(offset off)
	requires seekable_stream<S>;
	void seek_position(base_position base)
	requires seekable_stream<S>;
	void seek_position(base_position base, offset off)
	requires seekable_stream<S>;
	
	// Buffering
	void flush()
	requires buffered_stream<S>;
	
	// Reading
	streamsize read_some(span<byte> buffer)
	requires input_stream<S>;
	
	// Writing
	streamsize write_some(span<const byte> buffer)
	requires output_stream<S>;
private:
	mutable mutex m_mutex; ///< Mutex used for synchronization.
	S m_stream; ///< Wrapped stream.
};

}

#include "synchronized_stream.hpp"
