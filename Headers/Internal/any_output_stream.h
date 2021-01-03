/// \file
/// \brief Internal header file that describes the any_output_stream class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

class any_output_stream final : public Any::StreamBase
{
public:
	// Constructors and destructor
	constexpr any_output_stream() noexcept = default;
	template <output_stream S>
	constexpr any_output_stream(S&& s);
	template <output_stream S, typename... Args>
	requires constructible_from<S, Args...>
	constexpr explicit any_output_stream(in_place_type_t<S>, Args&&... args);
	constexpr any_output_stream(const any_output_stream& other) = default;
	constexpr any_output_stream(any_output_stream&& other) noexcept = default;
	constexpr ~any_output_stream() = default;
	
	// Assignment
	constexpr any_output_stream& operator=(const any_output_stream& other)
		= default;
	constexpr any_output_stream& operator=(any_output_stream&& other) noexcept
		= default;
	template <output_stream S>
	constexpr any_output_stream& operator=(S&& s);
	
	// Observers
	template <output_stream S>
	constexpr const S& get() const;
	template <output_stream S>
	constexpr S& get();
	
	// Modifiers
	template <output_stream S, typename... Args>
	requires constructible_from<S, Args...>
	constexpr void emplace(Args&&... args);
	template <output_stream S>
	requires movable<S>
	constexpr S release();
	constexpr void swap(any_output_stream& other) noexcept;
	
	// Writing
	constexpr streamsize write_some(span<const byte> buffer);
private:
	/// \brief Base class for type erased output stream implementation.
	/// \details The inheritance heirarchy is this:
	///
	/// Any::SharedBase -> Base -> Any::SharedImpl -> Impl
	///
	/// This class inherits from SharedBase and adds write_some pure virtual
	/// member function so it is possible to write bytes via type erasure.
	class Base : public Any::SharedBase
	{
	public:
		// Writing
		virtual streamsize write_some(span<const byte> buffer) = 0;
	};
	
	/// \brief Type erased output stream implementation.
	/// \details The inheritance heirarchy is this:
	///
	/// Any::SharedBase -> Base -> Any::SharedImpl -> Impl
	///
	/// This class template completes the implementation by providing Copy and
	/// write_some member functions.
	/// \tparam S Actual type of the stream.
	template <output_stream S>
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
		
		// Writing
		constexpr streamsize write_some(span<const byte> buffer) override;
	};
};

}

#include "any_output_stream.hpp"
