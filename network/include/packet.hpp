#ifndef PACKET_HPP
#define PACKET_HPP

#include <array>
#include <string>
#include "../../engine/include/input.hpp"

class Packet
{
public:
	Packet();
	~Packet();

	const input* data() const;
	int size() const;

	input* i = nullptr;

private:

};

#endif PACKET_HPP
