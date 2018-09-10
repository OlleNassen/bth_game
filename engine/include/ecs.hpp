#ifndef ECS_HPP
#define ECS_HPP
#include <array>
#include <vector>
//::.. authors ..:://
// Edvard
//Olle

namespace ecs
{
using entity = int;

template <typename T>
T& find(T data, entity* begin, entity* end, entity e)
{
	return data[end - std::find(begin, end, e)];
}
	
}

#endif