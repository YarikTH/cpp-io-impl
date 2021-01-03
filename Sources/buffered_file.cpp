/// \file
/// \brief Source file that contains implementation of the BufferedFile class.
/// \author Lyberta
/// \copyright BSLv1.

#include <Internal/buffered_file.h>

#include <Internal/write_raw.h>

namespace std::io
{

BufferedFile::BufferedFile() noexcept
	: m_buffer_mode{mode::read}
{
}

BufferedFile::BufferedFile(const filesystem::path& file_name, mode m,
	creation c)
	: m_file{file_name, m, c},
	m_buffer_mode{mode::read}
{
	auto handle = m_file.native_handle();
	m_buffer_storage.resize(Platform::GetBufferSize(handle));
}

BufferedFile::BufferedFile(native_handle_type handle)
	: m_file{handle},
	m_buffer_mode{mode::read}
{
	m_buffer_storage.resize(Platform::GetBufferSize(handle));
}

BufferedFile::BufferedFile(BufferedFile&& other)
	: m_file{move(other.m_file)},
	m_buffer_storage{move(other.m_buffer_storage)},
	m_buffer_stream{other.m_buffer_stream},
	m_buffer_mode{other.m_buffer_mode}
{
	other.m_buffer_stream = {};
}

BufferedFile::~BufferedFile()
{
	try
	{
		this->flush();
	}
	catch (...)
	{
	}
}

BufferedFile& BufferedFile::operator=(BufferedFile&& other)
{
	if (this == &other)
	{
		return *this;
	}
	m_file = move(other.m_file);
	m_buffer_storage = move(other.m_buffer_storage);
	m_buffer_stream = other.m_buffer_stream;
	m_buffer_mode = other.m_buffer_mode;
	other.m_buffer_stream = {};
	return *this;
}

position BufferedFile::get_position() const
{
	switch (m_buffer_mode)
	{
		case mode::read:
		{
			return m_file.get_position() -
				offset{ranges::ssize(m_buffer_stream.get_buffer())} +
				offset{m_buffer_stream.get_position().value()};
		}
		case mode::write:
		{
			return m_file.get_position() +
				offset{m_buffer_stream.get_position().value()};
		}
		default:
		{
			throw logic_error{"WTF"};
		}
	}
}

void BufferedFile::seek_position(position pos)
{
	this->flush();
	m_file.seek_position(pos);
}

void BufferedFile::seek_position(offset off)
{
	this->flush();
	m_file.seek_position(off);
}

void BufferedFile::seek_position(base_position base)
{
	this->flush();
	m_file.seek_position(base);
}

void BufferedFile::seek_position(base_position base, offset off)
{
	this->flush();
	m_file.seek_position(base, off);
}

void BufferedFile::flush()
{
	if (this->IsBufferEmpty())
	{
		return;
	}
	switch (m_buffer_mode)
	{
		case mode::read:
		{
			// When reading, the OS position is ahead of the user facing one
			// because of the buffer so we need to seek backwards to sync
			// positions. We use absolute seek to be more robust here.
			m_file.seek_position(this->get_position());
			// Discard the buffer. This will mark it as empty and force re-read
			// from file on the next read_some.
			m_buffer_stream.set_buffer({});
			return;
		}
		case mode::write:
		{
			// Get proper buffer to write to file.
			auto buffer = m_buffer_stream.get_buffer();
			// Make sure we write only bytes that were actually written and not
			// garbage until the end of the sector.
			buffer = buffer.first(m_buffer_stream.get_position().value());
			// Write the buffer to file. This will sync positions.
			std::io::write_raw(buffer, m_file);
			// Discard the buffer. This will make sure the next write operation
			// sets up the buffer so it is properly aligned to sectors again.
			m_buffer_stream.set_buffer({});
			return;
		}
	}
}

streamsize BufferedFile::read_some(span<byte> buffer)
{
	ptrdiff_t bytes_to_read = ranges::ssize(buffer);
	if (bytes_to_read == 0)
	{
		return 0;
	}
	if (this->IsBufferEmpty())
	{
		this->SetNewReadBuffer();
	}
	else if (m_buffer_mode == mode::write)
	{
		this->flush();
		this->SetNewReadBuffer();
	}
	return m_buffer_stream.read_some(buffer);
}

streamsize BufferedFile::write_some(span<const byte> buffer)
{
	ptrdiff_t bytes_to_write = ranges::ssize(buffer);
	if (bytes_to_write == 0)
	{
		return 0;
	}
	if (m_buffer_mode == mode::read)
	{
		// When reading, the OS position is ahead of the user facing one because
		// of the buffer so we need to seek backwards to sync positions. We use
		// absolute seek to be more robust here.
		m_file.seek_position(this->get_position());
		this->SetNewWriteBuffer();
	}
	else if (m_buffer_stream.get_position() ==
		position{ranges::ssize(m_buffer_stream.get_buffer())})
	{
		// Buffer is full.
		this->flush();
		this->SetNewWriteBuffer();
	}
	auto result = m_buffer_stream.write_some(buffer);
	return result;
}

BufferedFile::native_handle_type BufferedFile::native_handle()
{
	return m_file.native_handle();
}

void BufferedFile::assign(native_handle_type handle)
{
	this->flush();
	m_buffer_storage.resize(Platform::GetBufferSize(handle));
	m_file.assign(handle);
}

BufferedFile::native_handle_type BufferedFile::release()
{
	this->flush();
	return m_file.release();
}

bool BufferedFile::IsBufferEmpty() const noexcept
{
	switch (m_buffer_mode)
	{
		case mode::read:
		{
			return m_buffer_stream.get_position() == position{ranges::ssize(
				m_buffer_stream.get_buffer())};
		}
		case mode::write:
		{
			return m_buffer_stream.get_position() == position{};
		}
		default:
		{
			return true;
		}
	}
}

void BufferedFile::SetNewReadBuffer()
{
	auto temp_buffer = this->GetBufferUntilTheEndOfSector();
	auto result = m_file.read_some(temp_buffer);
	temp_buffer.first(result);
	m_buffer_stream.set_buffer(temp_buffer);
	m_buffer_mode = mode::read;
}

void BufferedFile::SetNewWriteBuffer()
{
	m_buffer_stream.set_buffer(this->GetBufferUntilTheEndOfSector());
	m_buffer_mode = mode::write;
}

streamoff BufferedFile::GetSectorOffset() const
{
	auto sector_size = ranges::ssize(m_buffer_storage);
	return m_file.get_position().value() % sector_size;
}

span<byte> BufferedFile::GetBufferUntilTheEndOfSector()
{
	auto sector_offset = this->GetSectorOffset();
	return {ranges::begin(m_buffer_storage) + sector_offset,
		ranges::end(m_buffer_storage)};
}

}
