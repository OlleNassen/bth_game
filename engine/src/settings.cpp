#include "settings.hpp"
#include <iostream>
Settings::Settings()
{
	importer = luaL_newstate();
	luaL_openlibs(importer);

	if (luaL_loadfile(importer, "../resources/scripts/setup/settings.lua") || lua_pcall(importer, 0, 0, 0))
		std::cout << lua_tostring(importer, -1) << '\n';

	lua_getglobal(importer, "settings");
	lua_getfield(importer, -1, "window");
	lua_getfield(importer, -1, "width");
	lua_getfield(importer, -2, "height");

	//lua_tointeger
	window.resolution.x = lua_tonumber(importer, -2);
	window.resolution.y = lua_tonumber(importer, -1);

	std::cout << window.resolution.x << " " << window.resolution.y << '\n';

}

Settings::~Settings()
{
	lua_close(importer);
}

