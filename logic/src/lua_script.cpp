#include "lua_script.hpp"
#include <functional>

namespace logic
{

PlayerScript::PlayerScript(const std::string& path)
	: stack{ path.c_str() }
{
	stack.setglobal("entities");
}

void PlayerScript::setup(int entity)
{
	std::string name{"entities[" + std::to_string(entity) + "]"};
	stack.newtable();
	stack.setglobal(name.c_str());

	stack.getglobal("setup");
	
	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal(name.c_str());
		stack.call(1, 0);
	}

	stack.clear();
}

void PlayerScript::update(
	std::chrono::milliseconds delta, 
	objects& object, 
	const input& i, 
	int index,
	const int& trigger,
	const int& type,
	anim& anim_state,
	int random_value,
	std::array<int, 4>& buff_activ,
	bool rw, bool lw)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	int top = stack.top();

	if (lua_istable(stack.lua_state, top))
	{
		{
			stack.push("button");
			stack.push(i);
			stack.rawset(top);
		}

		{
			stack.push("lw");
			stack.push(lw);
			stack.rawset(top);
			stack.push("rw");
			stack.push(rw);
			stack.rawset(top);
		}

		{
			
			stack.push("anim");
			stack.push(anim_state);
			stack.rawset(top);
		}

		{
			stack.push("position");
			stack.push(object.position);
			stack.rawset(top);
			stack.push("velocity");
			stack.push(object.velocity);
			stack.rawset(top);
			stack.push("size");
			stack.push(object.size);
			stack.rawset(top);
			stack.push("forces");
			stack.push(object.forces);
			stack.rawset(top);
			stack.push("impulse");
			stack.push(object.impulse);
			stack.rawset(top);
		}

		//test for trigger
		{
			stack.push("triggered");
			stack.push(trigger);
			stack.rawset(top);
		}

		{
			stack.push("triggered_type");
			stack.push(type);
			stack.rawset(top);
		}

		{
			stack.push("shield_active");
			stack.push(object.shield_active);
			stack.rawset(top);
		}

		{
			stack.push("random_value");
			stack.push(random_value);
			stack.rawset(top);
		}

		{
			stack.push("buff_activ");
			stack.push(buff_activ[index]);
			stack.rawset(top);
		}
		stack.clear();
	}

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal(name.c_str());
	stack.call(2, 0);

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		stack.getfield(top, "anim");
		stack.getfield(-1, "current");
		anim_state = static_cast<anim>(stack.tointeger(-1));
		stack.clear();
	}

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		
		stack.getfield(top, "position");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.position.x = stack.tonumber(-2);
		object.position.y = stack.tonumber(-1);

		stack.getfield(top, "velocity");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.velocity.x = stack.tonumber(-2);
		object.velocity.y = stack.tonumber(-1);

		stack.getfield(top, "size");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.size.x = stack.tonumber(-2);
		object.size.y = stack.tonumber(-1);

		stack.getfield(top, "forces");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.forces.x = stack.tonumber(-2);
		object.forces.y = stack.tonumber(-1);

		stack.getfield(top, "impulse");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.impulse.x = stack.tonumber(-2);
		object.impulse.y = stack.tonumber(-1);
		
		stack.getfield(top, "is_stund");
		object.is_stund = stack.toboolean(-1);

		stack.getfield(top, "shield_active");
		object.shield_active = stack.toboolean(-1);

		stack.getfield(top, "buff_activ");
		buff_activ[index] = stack.tonumber(-1);
		//if (buff_activ[index] > 0)
		//{
		//	std::cout << buff_activ[index] << std::endl;
		//}


		stack.clear();
	}	
}

bool PlayerScript::build_stage_done(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "done");

	bool temp = stack.toboolean(-1);

	stack.clear();

	return temp;
}

bool PlayerScript::build_stage_force_done(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "force_done");

	bool temp = stack.toboolean(-1);

	stack.clear();

	return temp;
}

void PlayerScript::set_build_stage_done(int index, bool state)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());

	int top = stack.top();
	stack.push("done");
	stack.push(state);
	stack.rawset(top);
	stack.clear();
}

float PlayerScript::get_time(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "time");

	float temp = stack.tonumber(-1);

	stack.clear();

	return temp;
}

double PlayerScript::dash_timer(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "dash_timer");

	double temp = stack.tonumber(-1);

	stack.clear();

	return temp;
}

GameScript::GameScript()
	: stack{ "../resources/scripts/gameloop.lua" }
{
	//stack.setglobal("entities");
}

void GameScript::setup()
{
	stack.newtable();
	stack.setglobal("game");
	stack.getglobal("setup");

	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal("game");
		stack.call(1, 0);
	}

	stack.newtable();
	stack.setglobal("entities");

	stack.getglobal("entities");
	int top = stack.top();
	for (int i = 1; i <= 4; i++)
	{
		stack.newtable();
		//std::string temp{ "p" + std::to_string(i) };
		stack.rawset(top, i);
		//stack.setglobal(temp.c_str());
	}

	stack.clear();
}

void GameScript::update(
	std::chrono::milliseconds delta,
	const input& i,
	const trigger_array& triggers,
	const trigger_type_array& types,
	objects* players,
	int player_count,
	int spike_frame, 
	int turret_frame,
	bool dash_active[],
	std::array<bool, 4> player_hit_array)
{
	{
		stack.getglobal("game");
		stack.clear();
	}
	
	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();
			stack.push("position");
			stack.push(players[i - 1].position);
			stack.rawset(top_pos);
		}

		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();
			stack.push("dash_active");
			stack.push(dash_active[i - 1]);
			stack.rawset(top_pos);
		}

		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();

			stack.push("triggered");
			stack.push(triggers[i - 1]);
			stack.rawset(top_pos);
		}
		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();

			stack.push("triggered_type");
			stack.push(types[i - 1]);
			stack.rawset(top_pos);
		}
		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();

			stack.push("impulse");
			stack.push(players[i - 1].impulse);
			stack.rawset(top_pos);
		}
		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();

			stack.push("velocity");
			stack.push(players[i - 1].velocity);
			stack.rawset(top_pos);
		}
		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();

			stack.push("shield_active");
			stack.push(players[i - 1].shield_active);
			stack.rawset(top_pos);
		}
		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; i++)
		{
			stack.rawget(top, i);
			int top_pos = stack.top();
	
			stack.push("laser_hit");
			stack.push(player_hit_array[i - 1]);
			stack.rawset(top_pos);
		}
		stack.clear();
	}

	{
		stack.getglobal("game");

		int top_pos = stack.top();

		stack.push("spike_frame");
		stack.push(spike_frame);
		stack.rawset(top_pos);

		stack.clear();
	}

	//{
	//	stack.getglobal("game");
	//
	//	int top_pos = stack.top();
	//
	//	stack.push("turret_frame");
	//	stack.push(turret_frame);
	//	stack.rawset(top_pos);
	//
	//	stack.clear();
	//}

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal("game");
	stack.getglobal("entities");
	stack.push(player_count);
	stack.call(4, 0);

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; ++i)
		{
			stack.rawget(top, i);
			stack.getfield(-1, "position");
			stack.getfield(-1, "x");
			stack.getfield(-2, "y");
			players[i - 1].position.x = stack.tonumber(-2);
			players[i - 1].position.y = stack.tonumber(-1);
		}

		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; ++i)
		{
			stack.rawget(top, i);
			stack.getfield(-1, "impulse");
			stack.getfield(-1, "x");
			stack.getfield(-2, "y");
			players[i - 1].impulse.x = stack.tonumber(-2);
			players[i - 1].impulse.y = stack.tonumber(-1);
		}

		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();

		for (int i = 1; i <= 4; ++i)
		{
			stack.rawget(top, i);
			stack.getfield(-1, "shield_active");
			players[i - 1].shield_active = stack.toboolean(-1);
		}

		stack.clear();
	}
}

void GameScript::update_export()
{
	stack.getglobal("game");
	int top = stack.top();

	for (int i = 1; i <= 4; ++i)
	{
		stack.getfield(top, "names");
		stack.rawget(-1, i);
		data.names[i - 1] = stack.tostring(-1);

		stack.getfield(top, "scores");
		stack.rawget(-1, i);
		data.scores[i - 1] = stack.tonumber(-1);

		stack.getfield(top, "died");
		stack.rawget(-1, i);
		data.died[i - 1] = stack.toboolean(-1);

		stack.getfield(top, "finished");
		stack.rawget(-1, i);
		data.finished[i - 1] = stack.toboolean(-1);

		stack.getfield(top, "triggered_type");
		stack.rawget(-1, i);
		data.trigger_type[i - 1] = stack.tointeger(-1);

		//std::cout<< data.died[i - 1] << '\n';
		//std::cout << data.finished[i - 1] << '\n';

	}

	stack.getfield(top, "winner");
	data.game_over = stack.toboolean(-1);

	stack.getfield(top, "goal");
	data.goal_height = stack.tonumber(-1);

	stack.clear();


}

float GameScript::get_time()
{
	std::string name{ "game" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "time");

	float temp = stack.tonumber(-1);

	stack.clear();

	return temp;
}

void GameScript::reset_time()
{
	{
		stack.getglobal("game");
	}

	stack.getglobal("reset_time");
	stack.getglobal("game");
	stack.call(1, 0);

	stack.clear();
}

PlacingScript::PlacingScript(const std::string& path)
	: stack{ path.c_str() }
{
	stack.setglobal("entities");
}

void PlacingScript::setup(int entity)
{
	std::string name{ "entities[" + std::to_string(entity) + "]" };
	stack.newtable();
	stack.setglobal(name.c_str());

	stack.getglobal("setup");

	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal(name.c_str());
		stack.call(1, 0);
	}

	stack.clear();
}

void PlacingScript::update(
	std::chrono::milliseconds delta,
	objects& object,
	const input& i,
	int index,
	anim& anim_state)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		stack.push("button");
		stack.push(i);
		stack.rawset(top);
		stack.clear();
	}

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		stack.push("anim");
		stack.push(anim_state);
		stack.rawset(top);
		stack.clear();
	}

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		stack.push("position");
		stack.push(object.position);
		stack.rawset(top);
		stack.push("velocity");
		stack.push(object.velocity);
		stack.rawset(top);
		stack.push("size");
		stack.push(object.size);
		stack.rawset(top);
		stack.push("forces");
		stack.push(object.forces);
		stack.rawset(top);
		stack.push("impulse");
		stack.push(object.impulse);
		stack.rawset(top);
		stack.clear();
	}

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal(name.c_str());
	stack.call(2, 0);

	{
		stack.getglobal(name.c_str());
		int top = stack.top();
		stack.getfield(top, "anim");
		stack.getfield(-1, "current");
		anim_state = static_cast<anim>(stack.tointeger(-1));
		stack.clear();
	}

	{
		stack.getglobal(name.c_str());
		int top = stack.top();

		stack.getfield(top, "position");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.position.x = stack.tonumber(-2);
		object.position.y = stack.tonumber(-1);

		stack.getfield(top, "velocity");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.velocity.x = stack.tonumber(-2);
		object.velocity.y = stack.tonumber(-1);

		stack.getfield(top, "size");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.size.x = stack.tonumber(-2);
		object.size.y = stack.tonumber(-1);

		stack.getfield(top, "forces");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.forces.x = stack.tonumber(-2);
		object.forces.y = stack.tonumber(-1);

		stack.getfield(top, "impulse");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.impulse.x = stack.tonumber(-2);
		object.impulse.y = stack.tonumber(-1);

		stack.clear();
	}
}

bool PlacingScript::build_stage_done(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "done");

	bool temp = stack.toboolean(-1);

	stack.clear();

	return temp;
}

bool PlacingScript::build_stage_force_done(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "force_done");

	bool temp = stack.toboolean(-1);

	stack.clear();

	return temp;
}

void PlacingScript::set_build_stage_done(int index, bool state)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());

	int top = stack.top();
	stack.push("done");
	stack.push(state);
	stack.rawset(top);
	stack.clear();
}

float PlacingScript::get_time(int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	stack.getfield(-1, "time");

	float temp = stack.tonumber(-1);

	stack.clear();

	return temp;
}

MovingPlatformsScript::MovingPlatformsScript(const std::string& path)
	: stack{ path.c_str() }
{
	stack.setglobal("entities");
}

void MovingPlatformsScript::setup(int entity)
{
	std::string name{ "entities[" + std::to_string(entity) + "]" };
	stack.newtable();
	stack.setglobal(name.c_str());

	stack.getglobal("setup");
	stack.push(entity);
	stack.getglobal(name.c_str());
	stack.call(2, 0);

	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal(name.c_str());
		stack.call(1, 0);
	}

	stack.clear();
}

void MovingPlatformsScript::update(
	std::chrono::milliseconds delta,
	objects& object,
	int index)
{
	std::string name{ "entities[" + std::to_string(index) + "]" };
	stack.getglobal(name.c_str());
	int top = stack.top();

	if (lua_istable(stack.lua_state, top))
	{

		{
			stack.push("position");
			stack.push(object.position);
			stack.rawset(top);
			stack.push("velocity");
			stack.push(object.velocity);
			stack.rawset(top);
		}
	}

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal(name.c_str());
	stack.call(2, 0);


	{
		stack.getglobal(name.c_str());
		int top = stack.top();

		stack.getfield(top, "position");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.position.x = stack.tonumber(-2);
		object.position.y = stack.tonumber(-1);

		stack.getfield(top, "velocity");
		stack.getfield(-1, "x");
		stack.getfield(-2, "y");
		object.velocity.x = stack.tonumber(-2);
		object.velocity.y = stack.tonumber(-1);

		stack.clear();
	}
}

}

