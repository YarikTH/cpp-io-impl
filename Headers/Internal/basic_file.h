/// \file
/// \brief Internal header file that describes the BasicFile class.
/// \author Lyberta
/// \copyright BSLv1.

#pragma once

#include "Utilities.h"

namespace std::io
{

/// \brief RAII wrapper around native file handle.
/// \details This class provides the absolute basic interface around native file
/// handle that only cares that it is closed correctly in the destructor.
/// Inheriting from this class allows derived classes to use Rule Of Zero.

class BasicFile
{
public:
	using native_handle_type = Platform::NativeHandle;
	
	// Construct/copy/destroy
	BasicFile() noexcept;
	BasicFile(native_handle_type handle);
	BasicFile(const BasicFile&) = delete;
	BasicFile(BasicFile&& other);
	~BasicFile();
	BasicFile& operator=(const BasicFile&) = delete;
	BasicFile& operator=(BasicFile&& other);
	
	// Native handle management
	native_handle_type native_handle() const noexcept;
	void assign(native_handle_type handle) noexcept;
	native_handle_type release() noexcept;
private:
	native_handle_type m_handle; ///< Native file handle.
};

}
