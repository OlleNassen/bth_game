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
	glm::ivec2 resolution;
	bool fullscreen;
};

class Settings
{
public:
	Settings();
	~Settings();
	const WindowSettings& get_window_settings()const;
	const GraphicsSettings& get_graphics_settings()const;

	void set_fullscreen();

private:
	void create();

	lua_State* importer;
	GraphicsSettings graphics;
	SoundSettings sound;
	WindowSettings window;
};



#endif