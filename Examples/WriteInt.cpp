#include <io>
#include <iostream>

int main()
{
	unsigned int value = 42;

	// Create a stream. This stream will write to dynamically allocated memory.
	std::io::output_memory_stream stream;
	
	// Create a context. Context contains format of non-byte data that is used
	// to correctly do [de]serialization. If stream answers the question
	// "Where?", context answers the question "How?".
	std::io::default_context context{stream};

	// Write the value to the stream.
	std::io::write(value, context);

	// Get reference to the buffer of the stream.
	const auto& buffer = stream.get_buffer();

	// Print the buffer.
	for (auto byte : buffer)
	{
		std::cout << std::to_integer<int>(byte) << ' ';
	}
	std::cout << '\n';
}
