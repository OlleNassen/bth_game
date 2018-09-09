#ifndef PACKET_HPP
#define PACKET_HPP

class packet
{
public:
	packet();
	~packet();

	char* data() const;
	int size() const;

private:

};

#endif PACKET_HPP
