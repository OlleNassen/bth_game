#include "lua_script.hpp"
#include <functional>

namespace logic
{

PlayerScript::PlayerScript()
	: stack{ "../resources/scripts/player.lua" }
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
	int index)
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

void GameScript::update(std::chrono::milliseconds delta,
	objects* players)
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

	stack.getglobal("update");
	stack.push(delta.count() / 1000.0f);
	stack.getglobal("game");
	stack.getglobal("entities");
	stack.call(3, 0);

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
}

std::array<PlayerResult, 4> GameScript::player_results()
{
	stack.getglobal("game");
	stack.getfield(-1, "scores");
	int top = stack.top();
	stack.rawget(top, 1);
	stack.rawget(top, 2);
	stack.rawget(top, 3);
	stack.rawget(top, 4);

	std::array<PlayerResult, 4> temp;

	int index = -4;
	for (int i = 0; i < 4; ++i)
	{
		temp[i] = PlayerResult{"P" + std::to_string(i + 1),
		stack.tonumber(index++)};
	}

	stack.clear();

	return temp;
}

}
