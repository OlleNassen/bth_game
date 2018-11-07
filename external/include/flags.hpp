#ifndef FLAGS_HPP
#define FLAGS_HPP


enum state : int
{
	waiting = 1 << 0,
	debug = 1 << 1,
	connected = 1 << 2,
	menu = 1 << 3,
	building = 1 << 4,
	chat = 1 << 5,
	render_physics = 1 << 6,
	playing = 1 << 7
};

enum class anim 
{
	start_jump,
	in_jump,
	falling,
	landing,
	hanging_wall,
	connect_wall,
	jump_from_wall,
	idle,
	running,
	turning,
	sliding,
	hanging_left,
	hanging_right,
	independent
};



#endif


