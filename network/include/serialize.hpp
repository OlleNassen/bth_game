#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include <cassert>

using uint64 = unsigned long long;
using uint32 = unsigned int;
using uint16 = unsigned short;
using uint8 = unsigned char;

constexpr uint32 pop_count(uint32 x)
{
	uint32 a = x - ((x >> 1)   & 0x55555555);
	uint32 b = (((a >> 2)      & 0x33333333) + (a & 0x33333333));
	uint32 c = (((b >> 4) + b) & 0x0f0f0f0f);
	uint32 d = c + (c >> 8);
	uint32 e = d + (d >> 16);
	return e & 0x0000003f;
};

constexpr int log2(uint32 x)
{
	uint32 a = x | (x >> 1);
	uint32 b = a | (a >> 2);
	uint32 c = b | (b >> 4);
	uint32 d = c | (c >> 8);
	uint32 e = d | (d >> 16);
	uint32 f = e >> 1;
	return pop_count(f);
};

constexpr int bits_required(int min, int max)
{
	return min == max ? 0 : log2(max - min) + 1;
}

struct buffer
{
	uint8* data; // pointer to buffer data
	int size;    // size of buffer data (bytes)
	int index;   // index of next byte to be read/written
};

void write_integer(buffer& buffer, uint32 value);
void write_short(buffer& buffer, uint16 value);
void write_char(buffer& buffer, uint8 value);

uint32 read_integer(buffer& buffer);
uint16 read_short(buffer& buffer);
uint8 read_char(buffer& buffer);

class bit_writer
{
public:
	void write_bits(uint32 value, int bits)
	{
		assert(bits > 0);
		assert(bits <= 32);
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
		
	uint32* buffer = nullptr;
	uint64 scratch = 0;
	int scratch_bits = 0;
	int word_index = 0;
	int num_words = 0;
	int bits_written = 0;
	int num_bits = 0;

private:

};

class bit_reader
{
public:
	uint32 read_bits(int bits)
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

	uint32* buffer = nullptr;
	uint64 scratch = 0;
	int scratch_bits = 0;
	int word_index = 0;
	int num_words = 0;
	int bits_read = 0;
	int num_bits = 0;	

private:

};

class write_stream
{
public:
	void serialize_integer(int value, int min, int max)
	{
		assert(min < max);
		assert(value >= min);
		assert(value <= max);
		const int bits = bits_required(min, max);

		uint32 unsigned_value = value - min;
		writer.write_bits(unsigned_value, bits);
	}

private:
	bit_writer writer;

};

class read_stream
{
public:
	int serialize_integer(int min, int max)
	{
		assert(min < max);
		const int bits = bits_required(min, max);

		uint32 unsigned_value = reader.read_bits(bits);
		return static_cast<int>(unsigned_value) + min;
	}

private:
	bit_reader reader;
};






#endif

