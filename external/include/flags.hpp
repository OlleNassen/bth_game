#ifndef FLAGS_HPP
#define FLAGS_HPP


enum state : int
{
	debug = 1 << 1,
	connected = 1 << 2,
	menu = 1 << 3,
	building = 1 << 4,
	chat = 1 << 5,
	render_physics = 1 << 6
};



#endif


