#include <io>
#include <iostream>

int main()
{
	// Some bytes we're gonna write to a file.
	std::array<std::byte, 4> initial_bytes{
		std::byte{1},
		std::byte{2},
		std::byte{3},
		std::byte{4}};

	{ // Start new RAII block.
		// Open a file for writing.
		std::io::output_file_stream stream{"test.bin"};
		// Write our bytes to the file.
		std::io::write_raw(initial_bytes, stream);
	} // End of RAII block. This will close the stream.
	
	// Create space for bytes to read from the file.
	std::array<std::byte, 4> read_bytes;
	
	{ // Start new RAII block.
		// Open the file again, but now for reading.
		std::io::input_file_stream stream{"test.bin"};
		// Read the bytes from the file.
		std::io::read_raw(read_bytes, stream);
	} // End of RAII block. This will close the stream.
	
	// Compare read bytes with initial ones.
	if (read_bytes == initial_bytes)
	{
		std::cout << "Bytes match.\n";
	}
	else
	{
		std::cout << "Bytes don't match.\n";
	}
}

