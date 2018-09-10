#ifndef PACKET_HPP
#define PACKET_HPP

#include <string>

class packet
{
public:
	packet();
	~packet();

	const char* data() const;
	int size() const;

	std::string s;

private:

};

#endif PACKET_HPP
