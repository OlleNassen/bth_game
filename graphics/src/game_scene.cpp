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
	//Level name 
	std::string str_file_name = (std::string)file_name;
	std::size_t found = str_file_name.find_last_of("/");
	level_name = str_file_name.substr(found + 1);

	//Create players
	CustomLevel level(file_name);
	CustomLevel objects("../resources/level/objects.ssp");
	models.clear();
	models.reserve(sizeof(Model) * (level.counterReader.levelObjectCount));

	using glm::vec3;
	glm::mat4 model{ 1.0f };

	v[0] = { 0, 1.75 };
	v[1] = { 3, 1.75 };
	v[2] = { 6, 1.75 };
	v[3] = { 9, 1.75 };

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
		moving_models[i].create_animation_data("Robot_dash.sspAnim", anim::dash);
		moving_models[i].create_animation_data("Robot_stun.sspAnim", anim::stun);


	}

	for (unsigned int i = 0; i < level.counterReader.levelObjectCount; i++)
	{
		glm::mat4 model{ 1.0f };

		model = glm::translate(model, glm::vec3(level.levelObjects[i].position[0], level.levelObjects[i].position[1], level.levelObjects[i].position[2]));
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[2]), glm::vec3{ 0,0,1 });
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[1]), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(level.levelObjects[i].rotation[0]), glm::vec3{ 1,0,0 });

		if (emissive_counter < 4)
		{
			current_emissive = 0;
		}
		else if (emissive_counter >= 4 && emissive_counter <= 9)
		{
			current_emissive = 1;
		}
		else if (emissive_counter >= 10)
		{
			current_emissive = 2;
		}

		if (level.levelObjects[i].id == 62)
		{
			models.emplace_back(model, emissive_colors[current_emissive], mesh_lib->get_mesh(level.levelObjects[i].id));
			emissive_counter++;
		}
		else
		{
			models.emplace_back(model, glm::vec3(1, 1, 1), mesh_lib->get_mesh(level.levelObjects[i].id));
		}

		if (models.back().is_animated)
		{
			if (level.levelObjects[i].id == 63)
			{
				animated_models.emplace_back(model, emissive_colors[current_emissive], mesh_lib->get_mesh(level.levelObjects[i].id));
				emissive_counter++;
			}
			else
			{
				animated_models.emplace_back(model, glm::vec3(1, 1, 1), mesh_lib->get_mesh(level.levelObjects[i].id));
			}
			models.pop_back();
		}

		if (level.levelObjects[i].position[2] > -0.01f && level.levelObjects[i].position[2] < 0.01f)
		{
			float width = level.levelObjects[i].collisionBox[1];
			float height = level.levelObjects[i].collisionBox[0];
			auto* ptr = level.levelObjects[i].centerPivot;

			coll_data.emplace_back(collision_data{
				glm::vec2{ ptr[0], ptr[1] }, width, height, false });
		}
	}

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

void GameScene::light_lobby()
{
	//Player Light
	lights[0].color = glm::vec3{ 0.9f, 0.1f, 0.1f };
	lights[0].intensity = 30;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;

	//Map Light
	lights[4].position = glm::vec3{ 0.0, 8.689, -23.873 };
	lights[4].color = glm::vec3{ 0, 0.82, 1 };
	lights[4].intensity = 600;
	lights[5].position = glm::vec3{ 0.0, 31.901, -3.596 };
	lights[5].color = glm::vec3{ 0, 0.82, 1 };
	lights[5].intensity = 600;
	lights[6].position = glm::vec3{ 12.361, 4.61, -32.308 };
	lights[6].color = glm::vec3{ 1, 1, 0 };
	lights[6].intensity = 5;
	lights[7].position = glm::vec3{ 16.868, 8.91, -26.007 };
	lights[7].color = glm::vec3{ 1, 1, 0 };
	lights[7].intensity = 5;
	lights[8].position = glm::vec3{ 17.924, 5.232, -22.665 };
	lights[8].color = glm::vec3{ 1, 1, 0 };
	lights[8].intensity = 5;
	lights[9].position = glm::vec3{ 19.108, 2.175, -14.395 };
	lights[9].color = glm::vec3{ 1, 1, 0 };
	lights[9].intensity = 5;
	lights[10].position = glm::vec3{ -11.225, 8.62, -38.192 };
	lights[10].color = glm::vec3{ 1, 1, 0 };
	lights[10].intensity = 5;
	lights[11].position = glm::vec3{ -15.115, 5.317, -30.962 };
	lights[11].color = glm::vec3{ 1, 1, 0 };
	lights[11].intensity = 5;
	lights[12].position = glm::vec3{ -16.604, 9.025, -28.182 };
	lights[12].color = glm::vec3{ 1, 1, 0 };
	lights[12].intensity = 5;
	lights[13].position = glm::vec3{ -17.894, 5.857, -19.358 };
	lights[13].color = glm::vec3{ 1, 1, 0 };
	lights[13].intensity = 5;
	lights[14].position = glm::vec3{ -19.09, 2.282, -11.537 };
	lights[14].color = glm::vec3{ 1, 1, 0 };
	lights[14].intensity = 5;
	lights[15].position = glm::vec3{ -13.985, -0.805, -8.626 };
	lights[15].color = glm::vec3{ 1, 1, 0 };
	lights[15].intensity = 5;
	lights[16].position = glm::vec3{ 15.491, -1.476, -7.988 };
	lights[16].color = glm::vec3{ 1, 1, 0 };
	lights[16].intensity = 5;
	lights[17].position = glm::vec3{ 0, 17.11, 25.838 };
	lights[17].color = glm::vec3{ 0, 0.82, 1 };
	lights[17].intensity = 600;

	for (int i = 0; i < 18; i++)
	{
		lights[i].radius = lights[i].intensity;
	}
	lights[4].radius = 40;
	lights[5].radius = 40;
	lights[17].radius = 40;
}

void GameScene::light_level_1()
{
	//Player Light
	lights[0].color = glm::vec3{ 0.9f, 0.1f, 0.1f };
	lights[0].intensity = 30;
	lights[0].radius = 30;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[1].radius = 30;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[2].radius = 30;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;
	lights[3].radius = 30;

	//Map Light
	lights[4].position = glm::vec3{ -0.145,-6.289,8.929 };
	lights[4].color = glm::vec3{ 1,0.2,0 };
	lights[5].position = glm::vec3{ -7.73,23.368,-22.735 };
	lights[5].color = glm::vec3{ 1,0.2,0 };
	lights[6].position = glm::vec3{ -7.74,44,-22.984 };
	lights[6].color = glm::vec3{ 1,0.2,0 };
	lights[7].position = glm::vec3{ 0, 123.035, -23.725 };
	lights[7].color = glm::vec3{ 0, 0.82, 1 };
	lights[8].position = glm::vec3{ 0, 187.484, -18.185 };
	lights[8].color = glm::vec3{ 1, 0.2, 0 };
	lights[9].position = glm::vec3{ 0, 208.400, -34.226 };
	lights[9].color = glm::vec3{ 1, 0.2, 0 };
	lights[10].position = glm::vec3{ 0, 260.169, 5.0 };
	lights[10].color = glm::vec3{ 1, 1, 1 };
	lights[11].position = glm::vec3{ 0, 88.553, -29.053 };
	lights[11].color = glm::vec3{ 1, 0.2, 0 };
	lights[12].position = glm::vec3{ 0, 141.359, -31.505 };
	lights[12].color = glm::vec3{ 1, 0.2, 0 };

	for (int i = 4; i < 13; i++)
	{
		lights[i].intensity = 400;
	}

	lights[6].intensity = 700;

	for (int i = 4; i < 13; i++)
	{
		lights[i].radius = 60;
	}

}

void GameScene::light_level_2()
{
	//Player Light
	lights[0].color = glm::vec3{ 0.9f, 0.1f, 0.1f };
	lights[0].intensity = 30;
	lights[0].radius = 30;
	lights[1].color = glm::vec3{ 0.2f, 0.9f, 0.1f };
	lights[1].intensity = 30;
	lights[1].radius = 30;
	lights[2].color = glm::vec3{ 0.1f, 0.1f, 0.9f };
	lights[2].intensity = 30;
	lights[2].radius = 30;
	lights[3].color = glm::vec3{ 0.9f, 0.8f, 0.1f };
	lights[3].intensity = 30;
	lights[3].radius = 30;

	//Map Light
	//Blue screen lights
	lights[4].position = glm::vec3{ 2.73, 120.368,-10.735 };
	lights[4].color = glm::vec3{ 0.2 ,0.56, 0.9 };
	lights[5].position = glm::vec3{ -0.24, 143,-26.984 };
	lights[5].color = glm::vec3{ 0.1, 0.7, 0.9 };

	//Robot Lights
	lights[6].position = glm::vec3{ 0, 190.484, -8.785 };
	lights[6].color = glm::vec3{ 0.3, 0.9, 1.0 };
	lights[7].position = glm::vec3{ 0, 198.484, -8.785 };
	lights[7].color = glm::vec3{ 0.3, 0.9, 1.0 };
	lights[8].position = glm::vec3{ 0, 206.484, -8.785 };
	lights[8].color = glm::vec3{ 0.3, 0.9, 1.0 };

	//Red
	lights[9].position = glm::vec3{ 0, 36.53, -24.053 };
	lights[9].color = glm::vec3{ 1, 0.01, 0.01 };
	lights[10].position = glm::vec3{ 0, 46.53, -24.053 };
	lights[10].color = glm::vec3{ 1, 0.01, 0.01 };
	//Green
	lights[11].position = glm::vec3{ 0, 224.53, -13.053 };
	lights[11].color = glm::vec3{ 0.1, 1.0, 0.01 };
	lights[12].position = glm::vec3{ 0, 241.53, -13.053 };
	lights[12].color = glm::vec3{ 0.1, 1.0, 0.01 };
	lights[13].position = glm::vec3{ 0, 6.53, -21.853 };
	lights[13].color = glm::vec3{ 0.1, 1.0, 0.01 };

	lights[4].intensity = 800;
	lights[4].radius = 90;
	lights[5].intensity = 800;
	lights[5].radius = 90;

	lights[6].intensity = 600;
	lights[6].radius = 100;
	lights[7].intensity = 600;
	lights[7].radius = 100;
	lights[8].intensity = 600;
	lights[8].radius = 100;

	lights[9].intensity = 400;
	lights[9].radius = 80;
	lights[10].intensity = 400;
	lights[10].radius = 80;

	lights[11].intensity = 400;
	lights[11].radius = 80;
	lights[12].intensity = 400;
	lights[12].radius = 80;
	lights[13].intensity = 400;
	lights[13].radius = 80;
}

void GameScene::inititate_object(CustomLevel& objects, MeshLib* object_lib)
{
	this->objects.clear();
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
		int model_type_id = i;

		Model tempModel = { model, glm::vec3(1, 1, 1), object_lib->get_mesh(i) };
		collision_data tempInfo = { glm::vec2{ ptr[0], ptr[1] }, width, height, false, model_type_id };
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

}


