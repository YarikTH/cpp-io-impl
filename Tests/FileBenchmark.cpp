#include <iostream>
#include <chrono>
#include <fstream>
#include <io>
#include <experimental/random>

class FILE_write_bench final
{
	std::FILE* m_file;
public:
	constexpr static std::string_view name = "std::FILE write";
	
	FILE_write_bench()
	{
		m_file = std::fopen("test_FILE.bin", "wb");
		if (m_file == nullptr)
		{
			throw std::runtime_error{"std::fopen() failed."};
		}
	}
	
	template <typename T>
	void Run(const T& data)
	{
		for (const auto& i : data)
		{
			if (std::fwrite(&i, sizeof(i), 1, m_file) < 1)
			{
				throw std::runtime_error{"std::fwrite() failed."};
			}
		}
		if (std::fclose(m_file) == EOF)
		{
			throw std::runtime_error{"std::fclose() failed."};
		}
	}
};

class FILE_read_bench final
{
	std::FILE* m_file;
public:
	constexpr static std::string_view name = "std::FILE read";
	
	FILE_read_bench()
	{
		m_file = std::fopen("test_FILE.bin", "rb");
		if (m_file == nullptr)
		{
			throw std::runtime_error{"std::fopen() failed."};
		}
	}
	
	template <typename T>
	void Run(const T& data)
	{
		typename T::value_type i;
		for (const auto& j : data)
		{
			if (std::fread(&i, sizeof(i), 1, m_file) < 1)
			{
				throw std::runtime_error{"std::fread() failed."};
			}
			if (i != j)
			{
				std::fclose(m_file);
				throw std::runtime_error{"Files don't match."};
			}
		}
	}
};

class filebuf_write_bench final
{
	std::filebuf m_file;
public:
	constexpr static std::string_view name = "std::filebuf write";
	
	filebuf_write_bench()
	{
		auto result = m_file.open("test_filebuf.bin",
			std::ios_base::out | std::ios_base::binary);
		if (result == nullptr)
		{
			throw std::runtime_error{"std::filebuf.open() failed."};
		}
	}
	
	template <typename T>
	void Run(const T& data)
	{
		std::streamsize bytes_written;
		for (const auto& i : data)
		{
			bytes_written = m_file.sputn(reinterpret_cast<const char*>(&i),
				sizeof(i));
			if (bytes_written < sizeof(i))
			{
				throw std::runtime_error{"std::filebuf.sputn() failed."};
			}
		}
		if (m_file.pubsync() == -1)
		{
			throw std::runtime_error{"std::filebuf.pubsync() failed."};
		}
	}
};

class filebuf_read_bench final
{
	std::filebuf m_file;
public:
	constexpr static std::string_view name = "std::filebuf read";
	
	filebuf_read_bench()
	{
		auto result = m_file.open("test_filebuf.bin",
			std::ios_base::in | std::ios_base::binary);
		if (result == nullptr)
		{
			throw std::runtime_error{"std::filebuf.open() failed."};
		}
	}
	
	template <typename T>
	void Run(const T& data)
	{
		typename T::value_type i;
		std::streamsize bytes_read;
		for (const auto& j : data)
		{
			bytes_read = m_file.sgetn(reinterpret_cast<char*>(&i), sizeof(i));
			if (bytes_read < sizeof(i))
			{
				throw std::runtime_error{"std::filebuf.sgetn() failed."};
			}
			if (i != j)
			{
				throw std::runtime_error{"Files don't match."};
			}
		}
	}
};

class ofstream_bench final
{
	std::ofstream m_stream;
public:
	constexpr static std::string_view name = "std::ofstream";
	
	ofstream_bench()
		: m_stream{"test_fstream.bin",
			std::ios_base::out | std::ios_base::binary}
	{
		m_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	}
	
	template <typename T>
	void Run(const T& data)
	{
		for (const auto& i : data)
		{
			m_stream.write(reinterpret_cast<const char*>(&i), sizeof(i));
		}
		m_stream.flush();
	}
};

class ifstream_bench final
{
	std::ifstream m_stream;
public:
	constexpr static std::string_view name = "std::ifstream";
	
	ifstream_bench()
		: m_stream{"test_fstream.bin",
			std::ios_base::in | std::ios_base::binary}
	{
		m_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	}
	
	template <typename T>
	void Run(const T& data)
	{
		typename T::value_type i;
		for (const auto& j : data)
		{
			m_stream.read(reinterpret_cast<char*>(&i), sizeof(i));
			if (i != j)
			{
				throw std::runtime_error{"Files don't match."};
			}
		}
	}
};

class output_file_stream_bench final
{
	std::io::output_file_stream m_stream;
	std::io::default_context<std::io::output_file_stream> m_context;
public:
	constexpr static std::string_view name = "Buffered std::io::output_file_stream";
	
	output_file_stream_bench()
		: m_stream{"test_file_stream.bin", std::io::creation::always_new},
		m_context{m_stream}
	{
	}
	
	template <typename T>
	void Run(const T& data)
	{
		for (const auto& i : data)
		{
			std::io::write(i, m_context);
		}
		m_stream.flush();
	}
};

class input_file_stream_bench final
{
	std::io::input_file_stream m_stream;
	std::io::default_context<std::io::input_file_stream> m_context;
public:
	constexpr static std::string_view name = "Buffered std::io::input_file_stream";
	
	input_file_stream_bench()
		: m_stream{"test_file_stream.bin"},
		m_context{m_stream}
	{
	}
	
	template <typename T>
	void Run(const T& data)
	{
		typename T::value_type i;
		for (const auto& j : data)
		{
			std::io::read(i, m_context);
			if (i != j)
			{
				throw std::runtime_error{"Files don't match."};
			}
		}
	}
};

template <typename B>
void Benchmark(const auto& data)
{
	std::cout << B::name;
	B b;
	auto start_time = std::chrono::high_resolution_clock::now();
	try
	{
		b.Run(data);
	}
	catch (std::exception& e)
	{
		std::cout << ": FAILED\n";
		std::cout << "Reason: " << e.what() << '\n';
		return;
	}
	catch (...)
	{
		std::cout << ": FAILED\n";
		std::cout << "Reason: Unknown exception.\n";
		return;
	}
	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> time_elapsed = end_time -
		start_time;
	std::cout << ": " << time_elapsed.count() << " ms\n";
}

int main()
{
	std::vector<std::size_t> numbers;
	numbers.resize(10'000'000);
	for (auto&& number : numbers)
	{
		number = std::experimental::randint<std::size_t>(
			std::numeric_limits<std::size_t>::min(),
			std::numeric_limits<std::size_t>::max());
	}
	Benchmark<FILE_write_bench>(numbers);
	Benchmark<FILE_read_bench>(numbers);
	Benchmark<filebuf_write_bench>(numbers);
	Benchmark<filebuf_read_bench>(numbers);
	Benchmark<ofstream_bench>(numbers);
	Benchmark<ifstream_bench>(numbers);
	Benchmark<output_file_stream_bench>(numbers);
	Benchmark<input_file_stream_bench>(numbers);
}
