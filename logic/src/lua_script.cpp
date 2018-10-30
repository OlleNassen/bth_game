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
	std::string game{"game"};
	stack.newtable();
	stack.setglobal(game.c_str());
	stack.getglobal("setup");

	if (lua_isfunction(stack.lua_state, -1))
	{
		stack.getglobal(game.c_str());
		stack.call(1, 0);
	}

	std::string name{"entities"};
	stack.newtable();
	stack.setglobal(name.c_str());

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
		std::string game{ "game" };
		stack.getglobal(game.c_str());
		stack.clear();
	}
	
	{
		std::string name{ "entities" };
		stack.getglobal(name.c_str());
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
}

std::array<std::tuple<std::string, int, float>, 4> GameScript::name_id_score()
{
	std::string game{"game"};
	stack.getglobal(game.c_str());
	stack.getfield(-1, "scores");
	stack.getfield(-1, "0");
	stack.getfield(-2, "1");
	stack.getfield(-3, "2");
	stack.getfield(-4, "3");

	std::array<std::tuple<std::string, int, float>, 4> temp;

	float index = -4;
	for (int i = 0; i < 4; ++i)
	{
		temp[i] = std::make_tuple("p" + std::to_string(i), i,
			stack.tonumber(index++));
	}

	stack.clear();

	return temp;
}

}
