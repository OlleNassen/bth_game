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

	//test for triggers
	{
		stack.getglobal("entities");
		int top = stack.top();
		stack.push("button");
		stack.push(i);
		stack.rawset(top);
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

			stack.push("forces");
			stack.push(players[i - 1].forces);
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

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; ++i)
		{
			stack.rawget(top, i);
			stack.getfield(-1, "velocity");
			stack.getfield(-1, "x");
			stack.getfield(-2, "y");
			players[i - 1].velocity.x = stack.tonumber(-2);
			players[i - 1].velocity.y = stack.tonumber(-1);


		}

		stack.clear();
	}

	{
		stack.getglobal("entities");
		int top = stack.top();
		for (int i = 1; i <= 4; ++i)
		{
			stack.rawget(top, i);
			stack.getfield(-1, "forces");
			stack.getfield(-1, "x");
			stack.getfield(-2, "y");
			players[i - 1].forces.x = stack.tonumber(-2);
			players[i - 1].forces.y = stack.tonumber(-1);


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
	}

	stack.getfield(top, "winner");
	data.game_over = stack.toboolean(-1);
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

}

