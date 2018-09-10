#include "packet.hpp"

packet::packet()
{

}

packet::~packet()
{

}

const char* packet::data() const
{
	return s.c_str();
}

int packet::size() const
{
	return s.length();
}