/// \file
/// \brief Internal header file that describes context concepts.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "stream_concepts.h"

namespace std::io
{

template <typename C>
concept context =
	stream<typename C::stream_type> &&
	requires(const C ctx)
	{
		{ctx.get_stream()} -> same_as<const typename C::stream_type&>;
		{ctx.get_format()} -> same_as<format>;
	} && requires(C ctx, format f)
	{
		{ctx.get_stream()} -> same_as<typename C::stream_type&>;
		ctx.set_format(f);
	};

template <typename C>
concept input_context = context<C> && input_stream<typename C::stream_type>;

template <typename C>
concept output_context = context<C> && output_stream<typename C::stream_type>;

}
