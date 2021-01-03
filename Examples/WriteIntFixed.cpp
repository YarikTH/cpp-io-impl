#include <cstdint>
#include <io>
#include <iostream>

// Do not compile on systems with non-8-bit bytes.
static_assert(CHAR_BIT == 8);

int main()
{
	std::uint32_t value = 42;
	
	std::io::output_memory_stream stream;
	
	// Create a context with specific binary format.
	// Here we want our data in the stream to be in big endian byte order.
	std::io::default_context context{stream, std::endian::big};

	// Write the value to the stream using our format.
	// This will perform endianness conversion on non-big-endian systems.
	std::io::write(value, context);

	const auto& buffer = stream.get_buffer();

	for (auto byte : buffer)
	{
		std::cout << std::to_integer<int>(byte) << ' ';
	}
	std::cout << '\n';
}
