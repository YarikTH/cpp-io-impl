/// \file
/// \brief Internal header file that describes the default_context class
/// template.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "stream_concepts.h"
#include "format.h"

namespace std::io
{

template <stream S>
class default_context final
{
public:
	using stream_type = S;
	
	// Constructor
	constexpr default_context(S& s, format f = {}) noexcept;
	
	// Stream
	constexpr S& get_stream() noexcept;
	constexpr const S& get_stream() const noexcept;
	
	// Format
	constexpr format get_format() const noexcept;
	constexpr void set_format(format f) noexcept;
private:
	S& m_stream;
	format m_format;
};

}

#include "default_context.hpp"
