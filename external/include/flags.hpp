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
	playing = 1 << 7,
	pre_playing = 1 << 8,
	lobby = 1 << 9,
	game_over = 1 << 10,
	pre_building = 1 << 11,
	score = 1 << 12,
	loading = 1 << 13
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
	independent,
	dash,
	stun
};



#endif


