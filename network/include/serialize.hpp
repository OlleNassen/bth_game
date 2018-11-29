#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <cstdint>
#include <cassert>

namespace network
{

using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

union FloatInt
{
    float float_value;
    uint32 int_value;
};

class bit_writer
{
public:
	bit_writer(uint32* buffer, int num_bits);
	void write_bits(uint32 value, int bits);
	void* data() const;
	int size() const;

private:
    uint32* buffer = nullptr;
	uint64 scratch = 0;
	int num_bits = 0;
	int scratch_bits = 0;
	int num_words = 0;
	int word_index = 0;
    int bits_written = 0;
};

bit_writer& operator<<(bit_writer& read, bool value);
bit_writer& operator<<(bit_writer& read, char value);
bit_writer& operator<<(bit_writer& read, short value);
bit_writer& operator<<(bit_writer& read, int value);
bit_writer& operator<<(bit_writer& read, float value);

class bit_reader
{
public:
	bit_reader(uint32* buffer, int num_bits);
	uint32 read_bits(int bits);
	void* data() const;
	int size() const;

private:
    uint32* buffer = nullptr;
    uint64 scratch = 0;
    int num_bits = 0;
    int scratch_bits = 0;
    int num_words = 0;
    int word_index = 0;
    int bits_read = 0;
};

bit_reader& operator>>(bit_reader& reader, bool& value);
bit_reader& operator>>(bit_reader& read, char& value);
bit_reader& operator>>(bit_reader& read, short& value);
bit_reader& operator>>(bit_reader& read, int& value);
bit_reader& operator>>(bit_reader& read, float& value);

}

#endif

