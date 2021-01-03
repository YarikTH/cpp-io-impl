Note: The author (Amanda Kornoushenko aka Lyberta) prepared a great proposal [P2146](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2146r0.pdf), but [declared loss of interest](https://github.com/cplusplus/papers/issues/860) in the progress.
Also [post on reddit](https://www.reddit.com/r/cpp/comments/fe72kp/modern_stdbyte_stream_io_for_c/) and [repository on githib](https://github.com/Lyberta/cpp-io-impl) were deleted. Fortunately [link to code archive](http://korno.net/cpp/P2146.tar.gz) has survived (not sure at which revision of proposal), so here just raw content of this archive. I think this project can be used as a base for a great binary io library that can work on top of c++17 or even less standard. \
IANAL, but the text of BSL says that "Permission is hereby granted... to use, reproduce, display, distribute, execute, and transmit the Software, and to prepare derivative works of the Software" so I'm sure that according to the license I'm allowed to publish this copy of the code even if original work is removed by the author.

# Implementation of P2146 - Modern std::byte stream IO for C++

Reference implementation of [P2146 - Modern std::byte stream IO for C++](https://wg21.link/p2146).

None of the major compilers support all of the C++20 features required to implement this proposal. The missing features are:

* GCC: `std::bit_cast`.
* Clang: concepts library, ranges, `std::bit_cast`.
* MSVC: ranges.

| Component                                 | Implementation status |
| ----------------------------------------- | --------------------- |
| std::io::make_error_code                  | ✅                     |
| std::io::make_error_condition             | ✅                     |
| std::io::category                         | Partial               |
| std::io::io_error                         | Partial               |
| std::io::offset                           | ✅                     |
| std::io::position                         | ✅                     |
| std::io::input_stream                     | ✅                     |
| std::io::output_stream                    | ✅                     |
| std::io::stream                           | ✅                     |
| std::io::input_output_stream              | ✅                     |
| std::io::seekable_stream                  | ✅                     |
| std::io::buffered_stream                  | ✅                     |
| std::io::read_raw                         | ✅                     |
| std::io::write_raw                        | ✅                     |
| std::io::format                           | ✅                     |
| std::io::context                          | ✅                     |
| std::io::input_context                    | ✅                     |
| std::io::output_context                   | ✅                     |
| std::io::default_context                  | ✅                     |
| std::io::read                             | Partial               |
| std::io::write                            | Partial               |
| std::io::readable_from                    | ✅                     |
| std::io::writable_to                      | ✅                     |
| std::io::any_input_stream                 | ✅                     |
| std::io::any_output_stream                | ✅                     |
| std::io::any_input_output_stream          | ✅                     |
| std::io::in                               | Partial               |
| std::io::out                              | Partial               |
| std::io::err                              | Partial               |
| std::io::input_span_stream                | ✅                     |
| std::io::output_span_stream               | ✅                     |
| std::io::input_output_span_stream         | ✅                     |
| std::io::basic_input_memory_stream        | ✅                     |
| std::io::basic_output_memory_stream       | ✅                     |
| std::io::basic_input_output_memory_stream | ✅                     |
| std::io::input_file_stream                | Partial               |
| std::io::output_file_stream               | Partial               |
| std::io::input_output_file_stream         | Partial               |

## Notes on floating point support

`std::io::floating_point_format::native` is supported everywhere.

Full `std::io::floating_point_format::iec559` functionality requires mainstream endianness, native ISO 60559 types and lack of padding bits.

* Only `binary32` and `binary64` formats without padding bits are supported right now.
* Assuming the previous point, `std::endian::native` is supported regardless of its value.
* Otherwise, only byte swaps are supported (little to big, big to little). This requires `std::endian::native` to be either `std::endian::little` or `std::endian::big`.

## Notes on file IO and standard stream objects

* Only POSIX and Windows are supported right now.
* If a POSIX or Windows call produces the error that is not in the proposal, `std::system_error` is thrown.

### POSIX specific notes

* `std::io::creation::always_new` does not have direct equivalent in POSIX and is implemented using `std::filesystem` which suffers from TOCTOU.

### Windows specific notes

* File buffer size is hardcoded to 4096.
* Standard stream objects are not thread-safe yet.

## How to build

General build dependencies:
* Git
* CMake 3.12 or newer

### Linux

#### Step 1: Create build directory and change into it

```
mkdir Build
cd Build
```

#### Step 2: Configure CMake with your choice of compiler and standard library

##### GCC and libstdc++

Requires at least GCC 10.2.

Assuming `g++-trunk` is not default compiler but it is the latest version you want:

```
cmake -D CMAKE_CXX_COMPILER=g++-trunk ..
```

##### Clang and libstdc++

Requires at least Clang 10 and libstdc++ shipped with GCC 10.2.

Assuming `clang++` is not default compiler but it is the latest version you want:

```
cmake -D CMAKE_CXX_COMPILER=clang++ ..
```

##### Clang and libc++

**Not usable right now because of lack of concepts library.**

Assuming `clang++` is not default compiler but it is the latest version you want:

```
cmake -D CMAKE_CXX_COMPILER=clang++ -D CMAKE_CXX_FLAGS=-stdlib=libc++ ..
```

#### Step 3: Build the library

```
cmake --build .
```

### Windows

#### Mingw-w64 on Linux (cross-compiling)

Requires at least GCC 10.2.

Assuming `x86_64-w64-mingw32-g++` is the compiler you want:

```
mkdir Build
cd Build
cmake -D CMAKE_SYSTEM_NAME=Windows -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ ..
cmake --build .
```

#### MSVC

Not supported yet.
