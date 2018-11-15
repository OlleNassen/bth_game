#include "game_scene.hpp"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

GameScene::GameScene()
{
}

GameScene::GameScene(const char* file_name, MeshLib* mesh_lib, MeshLib* object_lib)
{
	//Create players
	CustomLevel level(file_name);
	CustomLevel objects("../resources/level/objects.ssp");
	models.reserve(sizeof(Model) * (level.counterReader.levelObjectCount));

	using glm::vec3;
	glm::mat4 model{ 1.0f };

	v[0] = { 10, 15 };
	v[1] = { -3, 0 };
	v[2] = { 3, 0 };
	v[3] = { -9, 0 };

	moving_models.emplace_back(glm::translate(model, vec3{ v[0], 0 }), vec3{ 1.0f, 0.0f, 0.0f}, mesh_lib->get_mesh(0));
	moving_models.emplace_back(glm::translate(model, vec3{ v[1], 0 }), vec3{ 0.2f, 0.9f, 0.1f}, mesh_lib->get_mesh(0));
	moving_models.emplace_back(glm::translate(model, vec3{ v[2], 0 }), vec3{ 0.1f, 0.1f, 0.9f}, mesh_lib->get_mesh(0));
	moving_models.emplace_back(glm::translate(model, vec3{ v[3], 0 }), vec3{ 0.9f, 0.8f, 0.1f}, mesh_lib->get_mesh(0));

	for (int i = 0; i < 4; i++)
	{
		moving_models[i].create_animation_data("Robot_idle.sspAnim", anim::idle);
		moving_models[i].create_animation_data("robot_run.sspAnim", anim::running);
		moving_models[i].create_animation_data("Robot_Jump_1.sspAnim", anim::start_jump);
		moving_models[i].create_animation_data("Robot_Jump_2.sspAnim", anim::falling);
		moving_models[i].create_animation_data("Robot_Jump_3.sspAnim", anim::in_jump);
		moving_models[i].create_animation_data("Robot_Jump_4.sspAnim", anim::landing);
		moving_models[i].create_animation_data("Robot_turn.sspAnim", anim::turning);
		moving_models[i].create_animation_data("Robot_wj1.sspAnim", anim::connect_wall);
		moving_models[i].create_animation_data("Robot_hanging.sspAnim", anim::hanging_left);
		moving_models[i].create_animation_data("Robot_hanging.sspAnim", anim::hanging_right);
		moving_models[i].create_animation_data("Robot_wj2.sspAnim", anim::jump_from_wall);
		moving_models[i].create_animation_data("Robot_slide.sspAnim", anim::sliding);
	}

	for (unsigned int i = 0; i < level.counterReader.levelObjectCount; i++)
	{
		glm::mat4 model{ 1.0f };

		model = glm::translate(model, glm::vec3(level.levelObjects[i].position[0], level.levelObjects[i].position[1], level.levelObjects[i].position[2]));
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[2]), glm::vec3{ 0,0,1 });
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[1]), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[0]), glm::vec3{ 1,0,0 });

		models.emplace_back(model, glm::vec3(0, 0, 0), mesh_lib->get_mesh(level.levelObjects[i].id));
		if (models.back().is_animated)
		{
			animated_models.emplace_back(model, glm::vec3(0, 0, 0), mesh_lib->get_mesh(level.levelObjects[i].id));
			models.pop_back();
		}

		if(level.levelObjects[i].position[2] > -0.01f && level.levelObjects[i].position[2] < 0.01f)
		{ 
			float width = level.levelObjects[i].collisionBox[1];
			float height = level.levelObjects[i].collisionBox[0];
			auto* ptr = level.levelObjects[i].centerPivot;
			
			//coll_data.emplace_back(collision_data{ 
			//	glm::vec2{ ptr[0], ptr[1] }, width, height, false });

			coll_data_test.emplace_back(collision_data{ 
				glm::vec2{ ptr[0], ptr[1] }, width, height, false });
		}
	}

	coll_data = combineding_coll_data();

	auto beg = models.begin();
	beg += 9;

	std::sort(beg, models.end(), [](const auto& left, const auto& right)
	{
		return left.get_y_position() < right.get_y_position();
	});

	beg = animated_models.begin();
	std::sort(beg, animated_models.end(), [](const auto& left, const auto& right)
	{
		return left.get_y_position() < right.get_y_position();
	});

	// Lucas/Vincent Test för placering av object.
	inititate_object(objects, object_lib);
}

GameScene::~GameScene()
{
}

void GameScene::inititate_object(CustomLevel& objects, MeshLib* object_lib)
{
	for (unsigned int i = 0; i < objects.counterReader.levelObjectCount; i++)
	{
		glm::mat4 model{ 1.0f };

		model = glm::translate(model, glm::vec3(objects.levelObjects[i].position[0], objects.levelObjects[i].position[1], objects.levelObjects[i].position[2]));
		model = glm::rotate(model, glm::radians(objects.levelObjects[i].rotation[2]), glm::vec3{ 0,0,1 });
		model = glm::rotate(model, glm::radians(objects.levelObjects[i].rotation[1]), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(objects.levelObjects[i].rotation[0]), glm::vec3{ 1,0,0 });

		int id = objects.levelObjects[i].id;

		float width = objects.levelObjects[i].collisionBox[1];
		float height = objects.levelObjects[i].collisionBox[0];
		auto* ptr = objects.levelObjects[i].centerPivot;

		Model tempModel = { model, glm::vec3(0, 0, 0), object_lib->get_mesh(i) };
		collision_data tempInfo = { glm::vec2{ ptr[0], ptr[1] }, width, height, false };
		placeableObjectInfo info = { tempModel, tempInfo };
		this->objects.emplace_back(info);
	}
}

int GameScene::add_object(collision_data& physics_data, int id)
{
	physics_data = objects[id].data;
	moving_models.push_back(objects[id].model);

	return moving_models.size() - 1;
}

void GameScene::clear_object()
{
	while (moving_models.size() > 4)
	{
		moving_models.pop_back();
	}
}

void GameScene::rotate_object(int model_id)
{
	moving_models[model_id].rotate(90.0f);
}

std::vector<collision_data> GameScene::combineding_coll_data()
{

	std::vector<collision_data> sorted_coll_data = coll_data_test;

	std::vector<collision_data> new_coll_data;


	std::sort(sorted_coll_data.begin(), sorted_coll_data.end(), [](const auto& left, const auto& right)
	{
		return left.position.y < right.position.y || ((left.position.y <= right.position.y) && (left.position.x < right.position.x));
	});

	//std::sort(sorted_coll_data.begin(), sorted_coll_data.end(), [](const auto& left, const auto& right)
	//{
	//	return left.position.x < right.position.x;
	//});


	glm::vec2 temp_pos = {0.0, 0.0};
	float temp_height = 0;
	float temp_width = 0;
	float min_x = 0;
	float max_x = 0;

	float left_side = 0;
	float right_side = 0;

	float temp_x = 0;

	float nr_of_models = 1;

	float margin = 0.5;
	
	float index = 0;

	/*for (int i = index; i < sorted_coll_data.size() - 1; i++)
	{
		nr_of_models = 1;

		temp_pos = sorted_coll_data[i].position;
		temp_height = sorted_coll_data[i].height;
		temp_width = sorted_coll_data[i].width;

		min_x = sorted_coll_data[i].position.x;
		max_x = sorted_coll_data[i].position.x;

		for (int j = i; j < sorted_coll_data.size(); j++)
		{

			if ((sorted_coll_data[j].position.y - 0.1) <= sorted_coll_data[j + 1].position.y && (sorted_coll_data[j].position.y + 0.1) >= sorted_coll_data[j + 1].position.y)
			{
				if (temp_pos.y < sorted_coll_data[j + 1].position.y)
				{
					temp_pos.y = sorted_coll_data[j + 1].position.y;
				}

				if (min_x > sorted_coll_data[j + 1].position.x)
				{
					min_x = sorted_coll_data[j + 1].position.x;
				}
				else if (max_x < sorted_coll_data[j + 1].position.x)
				{
					max_x = sorted_coll_data[j + 1].position.x;
				}

				temp_width += sorted_coll_data[j + 1].width;

				nr_of_models++;
				index++;
			}

			if (((min_x + temp_width / 2) - 0.1) <= (sorted_coll_data[j + 1].position.x - (sorted_coll_data[j + 1].width / 2))
				|| (max_x - temp_width / 2) - 0.1) >= (sorted_coll_data[j + 1].position.x + sorted_coll_data[j + 1].width / 2)
			{

			}
		}

		if (nr_of_models > 1)
		{
			temp_pos.x = (max_x + min_x) / 2;
		}

		new_coll_data.push_back(collision_data{ temp_pos, temp_width, temp_height, false });
	}*/



	while (sorted_coll_data.size() > 0)
	{
		int i = 0;

		temp_pos = sorted_coll_data[i].position;
		temp_height = sorted_coll_data[i].height;
		temp_width = sorted_coll_data[i].width;

		min_x = sorted_coll_data[i].position.x;
		max_x = sorted_coll_data[i].position.x;

		temp_x = (min_x + max_x) / 2;

		left_side = temp_x - (temp_width / 2);
		right_side = temp_x + (temp_width / 2);

		collision_data temp_data;

		sorted_coll_data.erase(sorted_coll_data.begin() + i);

		//temp_data = sorted_coll_data[sorted_coll_data.size() - 1];
		//sorted_coll_data[sorted_coll_data.size() - 1] = sorted_coll_data[i];
		//sorted_coll_data[i] = temp_data;
		//
		//sorted_coll_data.pop_back();

		for (int j = 0; j < sorted_coll_data.size(); j++)
		{

			if ((temp_pos.y - margin) <= sorted_coll_data[j].position.y
				&& (temp_pos.y + margin) >= sorted_coll_data[j].position.y)
			{
				if ((left_side - margin) <= (sorted_coll_data[j].position.x + (sorted_coll_data[j].width / 2))
					&& (left_side + margin) >= (sorted_coll_data[j].position.x + (sorted_coll_data[j].width / 2)))
				{
					if (temp_pos.y < sorted_coll_data[j].position.y)
					{
						temp_pos.y = sorted_coll_data[j].position.y;
					}

					if (min_x > sorted_coll_data[j].position.x)
					{
						min_x = sorted_coll_data[j].position.x;
					}
					else if (max_x < sorted_coll_data[j].position.x)
					{
						max_x = sorted_coll_data[j].position.x;
					}

					temp_width += sorted_coll_data[j].width;
					temp_x = (min_x + max_x) / 2;

					left_side = temp_x - (temp_width / 2);
					right_side = temp_x + (temp_width / 2);

					//temp_data = sorted_coll_data[sorted_coll_data.size() - 1];
					//sorted_coll_data[sorted_coll_data.size() - 1] = sorted_coll_data[j];
					//sorted_coll_data[j] = temp_data;
					//
					//sorted_coll_data.pop_back();

					sorted_coll_data.erase(sorted_coll_data.begin() + j);

					j = 0;

				}
				else if ((right_side - margin) <= (sorted_coll_data[j].position.x - (sorted_coll_data[j].width / 2))
					&& (right_side + margin) >= (sorted_coll_data[j].position.x - (sorted_coll_data[j].width / 2)))
				{
					if (temp_pos.y < sorted_coll_data[j].position.y)
					{
						temp_pos.y = sorted_coll_data[j].position.y;
					}

					if (min_x > sorted_coll_data[j].position.x)
					{
						min_x = sorted_coll_data[j].position.x;
					}
					else if (max_x < sorted_coll_data[j].position.x)
					{
						max_x = sorted_coll_data[j].position.x;
					}

					temp_width += sorted_coll_data[j].width;

					temp_width += sorted_coll_data[j].width;
					temp_x = (min_x + max_x) / 2;

					left_side = temp_x - (temp_width / 2);
					right_side = temp_x + (temp_width / 2);

					//temp_data = sorted_coll_data[sorted_coll_data.size() - 1];
					//sorted_coll_data[sorted_coll_data.size() - 1] = sorted_coll_data[j];
					//sorted_coll_data[j] = temp_data;
					//
					//sorted_coll_data.pop_back();

					sorted_coll_data.erase(sorted_coll_data.begin() + j);

					j = 0;

				}
			}
		}

		temp_pos.x = temp_x;

		new_coll_data.push_back(collision_data{ temp_pos, temp_width, temp_height, false });
	}

	
	return new_coll_data;
	
}

}

