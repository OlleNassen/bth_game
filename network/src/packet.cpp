#include "packet.hpp"

packet::packet()
{

}

packet::~packet()
{

}

const input* packet::data() const
{
	return i;
}

int packet::size() const
{
	return sizeof(input) * 8;
}