#include "Packet.hpp"

Packet::Packet()
{

}

Packet::~Packet()
{

}

const input* Packet::data() const
{
	return i;
}

int Packet::size() const
{
	return sizeof(input) * 8;
}