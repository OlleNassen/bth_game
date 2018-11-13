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
	bool get_screen_mode() const;
	void create(bool);
private:

	lua_State* importer;
	GraphicsSettings graphics;
	SoundSettings sound;
	WindowSettings window;
};



#endif