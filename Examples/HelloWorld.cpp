#include <io>

int main()
{
	std::string string = "Hello world!\n";
	std::span<const std::byte> buffer{reinterpret_cast<const std::byte*>(
		std::ranges::data(string)), std::ranges::size(string)};
	
	std::io::write_raw(buffer, std::io::out());
}
