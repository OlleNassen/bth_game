#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include <glm/glm.hpp>
#include <vector>
#include "geometry3d.hpp"

namespace physics
{

class Rigidbody
{
public:
	void update(float delta_seconds);
	void apply_forces();
	void solve_constraints(const std::vector<OBB>& constraints);

};

}

#endif
