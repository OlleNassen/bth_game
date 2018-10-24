#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include "lua.hpp"
#include <glm/glm.hpp>

struct GraphicsSettings
{

};

struct SoundSettings
{

};

struct WindowSettings
{
	glm::vec2 resolution;
	bool fullscreen;
};

class Settings
{
public:
	Settings();
	~Settings();

private:
	lua_State* importer;
	GraphicsSettings graphics;
	SoundSettings sound;
	WindowSettings window;
};



#endif