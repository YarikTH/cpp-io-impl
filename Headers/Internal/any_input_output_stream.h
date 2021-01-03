/// \file
/// \brief Internal header file that describes the any_input_output_stream
/// class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "any_stream_utilities.h"

namespace std::io
{

class any_input_output_stream final : public Any::StreamBase
{
public:
	// Constructors and destructor
	constexpr any_input_output_stream() noexcept = default;
	template <input_output_stream S>
	constexpr any_input_output_stream(S&& s);
	template <input_output_stream S, typename... Args>
	requires constructible_from<S, Args...>
	constexpr explicit any_input_output_stream(in_place_type_t<S>,
		Args&&... args);
	constexpr any_input_output_stream(const any_input_output_stream& other)
		= default;
	constexpr any_input_output_stream(any_input_output_stream&& other) noexcept
		= default;
	constexpr ~any_input_output_stream() = default;
	
	// Assignment
	constexpr any_input_output_stream& operator=(
		const any_input_output_stream& other) = default;
	constexpr any_input_output_stream& operator=(
		any_input_output_stream&& other) noexcept = default;
	template <input_output_stream S>
	constexpr any_input_output_stream& operator=(S&& s);
	
	// Observers
	template <input_output_stream S>
	constexpr const S& get() const;
	template <input_output_stream S>
	constexpr S& get();
	
	// Modifiers
	template <input_output_stream S, typename... Args>
	requires constructible_from<S, Args...>
	constexpr void emplace(Args&&... args);
	template <input_output_stream S>
	requires movable<S>
	constexpr S release();
	constexpr void swap(any_input_output_stream& other) noexcept;
	
	// Reading
	constexpr streamsize read_some(span<byte> buffer);
	
	// Writing
	constexpr streamsize write_some(span<const byte> buffer);
private:
	/// \brief Base class for type erased input and output stream
	/// implementation.
	/// \details The inheritance heirarchy is this:
	///
	/// Any::SharedBase -> Base -> Any::SharedImpl -> Impl
	///
	/// This class inherits from SharedBase and adds read_some and write_some
	/// pure virtual member functions so it is possible to read and write bytes
	/// via type erasure.
	class Base : public Any::SharedBase
	{
	public:
		// Reading
		virtual streamsize read_some(span<byte> buffer) = 0;
		
		// Writing
		virtual streamsize write_some(span<const byte> buffer) = 0;
	};
	
	/// \brief Type erased input and output stream implementation.
	/// \details The inheritance heirarchy is this:
	///
	/// Any::SharedBase -> Base -> Any::SharedImpl -> Impl
	///
	/// This class template completes the implementation by providing Copy,
	/// read_some and write_some member functions.
	/// \tparam S Actual type of the stream.
	template <input_output_stream S>
	class Impl final : public Any::SharedImpl<Base, S>
	{
	public:
		/// \brief Constructs itself from the copy of the stream.
		/// \param[in] s stream to copy.
		constexpr Impl(const S& s)
		requires copyable<S>;
		
		/// \brief Constructs itself by moving the stream inside.
		/// \param[in,out] s stream to move.
		constexpr Impl(S&& s)
		requires movable<S>;
		
		/// \brief Constructs itself by constructing contained stream in place.
		/// \tparam Args Types of the arguments to construct the stream from.
		/// \param[in] args Arguments to construct the stream from.
		template <typename... Args>
		requires constructible_from<S, Args...>
		constexpr Impl(in_place_t, Args&&... args);
		
		/// \brief Returns a pointer to the copy of *this.
		/// \return Pointer to the copy of *this.
		/// \throw std::io::io_error If wrapped stream is not copyable.
		constexpr Any::SharedBase* Copy() const override;
		
		// Reading
		constexpr streamsize read_some(span<byte> buffer) override;
		
		// Writing
		constexpr streamsize write_some(span<const byte> buffer) override;
	};
};

}

#include "any_input_output_stream.hpp"
