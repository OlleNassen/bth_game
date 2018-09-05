#ifndef ENTITY_COMPONENT_SYSTEM_HPP
#define ENTITY_COMPONENT_SYSTEM_HPP

//::.. authors ..:://
// Edvard

namespace ecs
{
using entity = int;

template <typename T>
std::tuple<std::vector<int>, std::vector<matrix>>

T& find(const std:tuple<std::vector<int>, std::vector<matrix>>& v, int e);

}

#endif