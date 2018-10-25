#include "settings.hpp"
#include <iostream>
#include <fstream>

Settings::Settings()
{
	importer = luaL_newstate();
	luaL_openlibs(importer);

	if (luaL_loadfile(importer, "../resources/scripts/setup/settings.lua") || lua_pcall(importer, 0, 0, 0))
	{
		create();
		if (luaL_loadfile(importer, "../resources/scripts/setup/settings.lua") || lua_pcall(importer, 0, 0, 0))
		{
			std::cout << lua_tostring(importer, -1) << '\n';
		}
	}

	lua_getglobal(importer, "settings");
	lua_getfield(importer, -1, "window");
	lua_getfield(importer, -1, "width");
	lua_getfield(importer, -2, "height");

	window.resolution.x = lua_tointeger(importer, -2);
	window.resolution.y = lua_tointeger(importer, -1);

	lua_getfield(importer, -3, "fullscreen");
	window.fullscreen = lua_toboolean(importer, -1);

	//std::cout << window.resolution.x << " " << window.resolution.y << '\n';
	//std::cout << window.fullscreen << '\n';

	//Clear stack
	lua_pop(importer, lua_gettop(importer));
}

Settings::~Settings()
{
	lua_close(importer);
}

const WindowSettings& Settings::get_window_settings()const
{
	return window;
}

const GraphicsSettings & Settings::get_graphics_settings() const
{
	return graphics;
}

void Settings::create()
{
	std::ofstream out("../resources/scripts/setup/settings.lua");

	std::string tab = "    ";

	out << "settings = \n" << "{\n" << "    window = \n" << "    {\n" << "        fullscreen = false,\n"
		<< "        width = 1280,\n" << "        height = 720,\n" << "    },\n\n" << "    sound =\n" << "    {\n"
		<< "        quality = 9.0 -- 10 is max\n" << "    },\n\n" << "    graphics =\n"
		<< "    {\n" << "        debug_fov = 90.0,\n" << "        depth_of_field = true\n" << "    }\n"
		<< "}\n";
}

