#ifndef PACKET_HPP
#define PACKET_HPP

#include <array>
#include <string>
#include "../../engine/include/input.hpp"

class packet
{
public:
	packet();
	~packet();

	const input* data() const;
	int size() const;

	input* i = nullptr;

private:

};

#endif PACKET_HPP
