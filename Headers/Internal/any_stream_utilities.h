/// \file
/// \brief Internal header file that describes the utility classes and functions
/// for type erased streams.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "stream_concepts.h"

namespace std::io::Any
{

/// \brief Shared base class for all type erased implementations.
/// \details The inheritance heirarchy is this:
///
/// Any::SharedBase -> Base -> Any::SharedImpl -> Impl
///
/// This class starts the inheritance heirarchy. It declares pure virtual
/// functions that all implementations must have which is everything except
/// reading and writing.

class SharedBase
{
public:
	/// \brief Virtual destructor for safe inheritance.
	virtual ~SharedBase() = default;
	
	/// \brief Returns a pointer to the copy of the most-derived class.
	/// \return Pointer to the copy of the most-derived class.
	/// \throw std::io::io_error If wrapped stream is not copyable.
	virtual SharedBase* Copy() const = 0;
	
	// Observers
	virtual const type_info& type() const noexcept = 0;
	
	// Position
	virtual position get_position() const = 0;
	virtual void seek_position(position pos) = 0;
	virtual void seek_position(offset off) = 0;
	virtual void seek_position(base_position base) = 0;
	virtual void seek_position(base_position base, offset off) = 0;
	
	// Buffering
	virtual void flush() = 0;
};

/// \brief Shared type erased implementation.
/// \details The inheritance heirarchy is this:
///
/// Any::SharedBase -> Base -> Any::SharedImpl -> Impl
///
/// This class template inherits from last non-template base class, contains the
/// actual stream that it wraps as a protected member and implements virtual
/// functions that all implementations must have which is everything except
/// reading and writing.
/// \tparam B Class to inherit from.
/// \tparam S Actual type of the stream.

template <typename B, stream S>
class SharedImpl : public B
{
public:
	/// \brief Constructs itself from the copy of the stream.
	/// \param[in] s stream to copy.
	constexpr SharedImpl(const S& s)
	requires copyable<S>;
	
	/// \brief Constructs itself by moving the stream inside.
	/// \param[in,out] s stream to move.
	constexpr SharedImpl(S&& s)
	requires movable<S>;
	
	/// \brief Constructs itself by constructing contained stream in place.
	/// \tparam Args Types of the arguments to construct the stream from.
	/// \param[in] args Arguments to construct the stream from.
	template <typename... Args>
	requires constructible_from<S, Args...>
	constexpr SharedImpl(in_place_t, Args&&... args);
	
	// Observers
	constexpr const type_info& type() const noexcept override;
	constexpr const S& get() const noexcept;
	constexpr S& get() noexcept;
	
	// Modifiers
	constexpr S release() noexcept;
	
	// Position
	constexpr position get_position() const override;
	constexpr void seek_position(position pos) override;
	constexpr void seek_position(offset off) override;
	constexpr void seek_position(base_position base) override;
	constexpr void seek_position(base_position base, offset off) override;
	
	// Buffering
	constexpr void flush() override;
protected:
	S m_stream; ///< Internal stream.
};

/// \brief Base class for type erased streams.
/// \details This class holds the pointer to the implementation and provides
/// public interface that is shared between all type erased streams.

class StreamBase
{
public:
	// Constructors and destructor
	constexpr StreamBase() noexcept;
	constexpr StreamBase(SharedBase* impl);
	constexpr StreamBase(const StreamBase& other);
	constexpr StreamBase(StreamBase&& other) noexcept;
	constexpr ~StreamBase();
	
	// Assignment
	constexpr StreamBase& operator=(const StreamBase& other);
	constexpr StreamBase& operator=(StreamBase&& other) noexcept;
	
	// Observers
	constexpr bool has_value() const noexcept;
	constexpr const type_info& type() const noexcept;
	
	// Modifiers
	constexpr void reset() noexcept;
	constexpr void swap(StreamBase& other) noexcept;
	
	// Position
	constexpr position get_position() const;
	constexpr void seek_position(position pos);
	constexpr void seek_position(offset off);
	constexpr void seek_position(base_position base);
	constexpr void seek_position(base_position base, offset off);
	
	// Buffering
	constexpr void flush();
protected:
	SharedBase* m_impl; ///< Pointer to the type erased implementation.
};

/// \brief Checks if the pointer to the stream is not null.
/// \param[in] ptr Pointer to check.
/// \throw std::io::io_error If pointer is null.
constexpr void CheckStream(auto& ptr);

}

#include "any_stream_utilities.hpp"
