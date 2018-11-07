#ifndef PLAYER_ARROWS_HPP
#define PLAYER_ARROWS_HPP

#include "user_interface.hpp"
#include "texture.hpp"
#include "primitives.hpp"

namespace graphics
{

class PlayerArrows : public UserInterface
{

public:
	PlayerArrows();
	~PlayerArrows() = default;

private:
	Texture texture;
	
};
}
#endif

