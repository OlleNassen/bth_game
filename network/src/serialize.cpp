#include "serialize.hpp"
#include <iostream>

namespace network
{

bit_writer::bit_writer(uint32* buffer, int num_bits)
    : buffer{buffer}
    , num_bits{num_bits}
    , num_words{num_bits * 32}
{

}

void bit_writer::write_bits(uint32 value, int bits)
{
    assert(bits > 0);
    assert(bits <= 32);
	std::cout << bits << '\n';
    assert(bits_written + bits <= num_bits);

    value &= (static_cast<uint64>(1) << bits) - 1;
    scratch |= static_cast<uint64>(value) << scratch_bits;
    scratch_bits += bits;

    if (scratch_bits >= 32)
    {
        assert(word_index < num_words);
        buffer[word_index] = static_cast<uint32>(scratch & 0xFFFFFF);
        scratch >>= 32;
        scratch_bits -= 32;
        ++word_index;
    }

    bits_written += bits;
}

void* bit_writer::data() const
{
	return buffer;
}

int bit_writer::size() const
{
	return bits_written * 8;
}

bit_writer& operator<<(bit_writer& writer, bool value)
{
    uint32 unsigned_value = value;
    writer.write_bits(unsigned_value, 1);
    return writer;
}

bit_writer& operator<<(bit_writer& writer, char value)
{
    uint32 unsigned_value = value;
    writer.write_bits(unsigned_value, 8);
    return writer;
}

bit_writer& operator<<(bit_writer& writer, short value)
{
    uint32 unsigned_value = value;
    writer.write_bits(unsigned_value, 16);
    return writer;
}

bit_writer& operator<<(bit_writer& writer, int value)
{
    uint32 unsigned_value = value;
    writer.write_bits(unsigned_value, 32);
    return writer;
}

bit_writer& operator<<(bit_writer& writer, float value)
{
    FloatInt temp;
    temp.float_value = value;
    writer.write_bits(temp.int_value, 32);
    return writer;
}


bit_reader::bit_reader(uint32* buffer, int num_bits)
    : buffer{buffer}
    , num_bits{num_bits}
    , num_words{num_bits * 32}
{

}

uint32 bit_reader::read_bits(int bits)
{
	assert(bits > 0);
	assert(bits <= 32);
	assert(bits_read + bits <= num_bits);

	bits_read += bits;

	assert(scratch_bits >= 0 && scratch_bits <= 64);

	if (scratch_bits < bits)
	{
		assert(word_index < num_words);
		scratch |= static_cast<uint64>(buffer[word_index]) << scratch_bits;
		scratch_bits += 32;
		++word_index;
	}

	assert(scratch_bits >= bits);

	const uint32 output = scratch & ((static_cast<uint64>(1) << bits) - 1);

	scratch >>= bits;
	scratch_bits -= bits;

	return output;
}

void* bit_reader::data() const
{
	return buffer;
}

int bit_reader::size() const
{
	return bits_read * 8;
}

bit_reader& operator>>(bit_reader& reader, bool& value)
{
    uint32 unsigned_value = reader.read_bits(1);
    value = static_cast<bool>(unsigned_value);
    return reader;
}

bit_reader& operator>>(bit_reader& reader, char& value)
{
    uint32 unsigned_value = reader.read_bits(8);
    value = static_cast<char>(unsigned_value);
    return reader;
}

bit_reader& operator>>(bit_reader& reader, short& value)
{
    uint32 unsigned_value = reader.read_bits(16);
    value = static_cast<short>(unsigned_value);
    return reader;
}

bit_reader& operator>>(bit_reader& reader, int& value)
{
    uint32 unsigned_value = reader.read_bits(32);
    value = static_cast<int>(unsigned_value);
    return reader;
}

bit_reader& operator>>(bit_reader& reader, float& value)
{
    FloatInt temp;
    temp.int_value = reader.read_bits(32);
    value = static_cast<float>(temp.float_value);
    return reader;
}

}

