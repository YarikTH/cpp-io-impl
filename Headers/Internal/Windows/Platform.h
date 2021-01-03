/// \file
/// \brief Internal header file that setups the Windows namespace.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

namespace std::io
{

// Introducing Windows namespace here.
namespace Windows
{
};

// With this alias cross-platform code can just refer to namespace
// std::io::Platform and avoid preprocessor.
namespace Platform = Windows;

}
