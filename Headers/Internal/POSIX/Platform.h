/// \file
/// \brief Internal header file that setups the POSIX namespace.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

// Introducing POSIX namespace here.
namespace POSIX
{
};

// With this alias cross-platform code can just refer to namespace
// std::io::Platform and avoid preprocessor.
namespace Platform = POSIX;

}
