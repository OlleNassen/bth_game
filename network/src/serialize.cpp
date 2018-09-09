#include "serialize.hpp"

int main()
{
	return 0;
}

void write_integer(buffer& buffer, uint32 value)
{
	assert(buffer.index + 4 <= buffer.size);

	*((uint32*)(buffer.data + buffer.index)) = value;
}

void write_short(buffer& buffer, uint16 value)
{

}

void write_char(buffer& buffer, uint8 value)
{

}

uint32 read_integer(buffer& buffer)
{
	assert(buffer.index + 4 <= buffer.size);

	uint32 value = *((uint32*)(buffer.data + buffer.index));
	buffer.index += 4;

	return value;
}

uint16 read_short(buffer& buffer)
{
	return 5;
}

uint8 read_char(buffer& buffer)
{
	return 5;
}