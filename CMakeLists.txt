cmake_minimum_required(VERSION 3.12)

# Name of the library.
set(LIBRARY_NAME stdio)

# Name of the library file.
set(LIBRARY_FILENAME stdio)

# Name of the project.
project(${LIBRARY_NAME} CXX)

# Adding a library target.
add_library(Library
	Sources/basic_file.cpp
	Sources/buffered_file.cpp
	Sources/file.cpp
	Sources/file_stream_base.cpp
	Sources/input_file_stream.cpp
	Sources/input_output_file_stream.cpp
	Sources/io_error.cpp
	Sources/output_file_stream.cpp
	Sources/special_file.cpp
	Sources/standard_streams.cpp)

# Setting the name of the library file.
set_target_properties(Library PROPERTIES OUTPUT_NAME ${LIBRARY_FILENAME})

# Specifying compiler configuration for the library.
target_compile_features(Library PUBLIC cxx_std_20)
#target_compile_options(Library PUBLIC "-fconcepts")
set_target_properties(Library PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Specifying include directories of the library.
target_include_directories(Library PUBLIC Headers)

# Adding platform-specific files.
if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
	target_sources(Library PRIVATE
		Sources/POSIX/StandardStreams.cpp
		Sources/POSIX/Utilities.cpp)
	target_include_directories(Library PUBLIC Headers/Internal/POSIX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_sources(Library PRIVATE
		Sources/Windows/StandardStreams.cpp
		Sources/Windows/Utilities.cpp)
	target_include_directories(Library PUBLIC Headers/Internal/Windows)
endif()

# Adding examples.
add_subdirectory(Examples)

# Adding tests.
add_subdirectory(Tests)
