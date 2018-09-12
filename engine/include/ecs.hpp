#ifndef ECS_HPP
#define ECS_HPP

#include <algorithm>

//::.. authors ..:://
// Edvard
//Olle

namespace ecs
{
using entity = int;

int find(entity* begin, entity* end, entity e)
{
	return end - std::find(begin, end, e);
}

int find_empty(entity* begin, entity* end)
{
	return find(begin, end, 0);
}

template <typename T>
const T& find(const T* components, entity* begin, entity* end, entity e)
{
	return components[find(begin, end, e)];
}
	
}

#endif