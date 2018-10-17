#ifndef RANGE_HPP
#define RANGE_HPP


#include <array>
#include <vector>

template <typename T>
class range
{
public:
	range() = default;
	
	template <int N>
	range(std::array<T, N>& data)
		: first{&data[0]}
		, last{&data[data.size()]} {}

	range(std::vector<T>& data)
		: first{ &data[0] }
		, last{ &data[data.size()] } {}

	int size() const { return last - first; }

	const T* begin() const { return first; }
	const T* end() const { return last; }
	const T& operator[](int i) const { return first[i]; }

	T* begin() { return first; }
	T* end() { return last; }
	T& operator[](int i) { return first[i]; }


private:
	T* first = nullptr;
	T* last = nullptr;
};


#endif
