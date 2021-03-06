#include "scene_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include <input/keyboard.h>
#include<input/touch_input_manager.h>


SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform)
	, sprite_renderer_(NULL)
	, renderer_3d_(NULL)
	, primitive_builder_(NULL)
	, font_(NULL)
	, scene_assets_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);

	// create the renderer for draw 3D geometry
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	input_manager_ = gef::InputManager::Create(platform_);

	// initialise primitive builder to make create some 3D geometry easier
	primitive_builder_ = new PrimitiveBuilder(platform_);

	// initialise the physics world
	b2Vec2 gravity(0.0f, -9.81f);
	world = new b2World(gravity);

	// load the assets in from the .scn
	const char* scene_asset_filename = "apple.scn";
	scene_assets_ = LoadSceneAssets(platform_, scene_asset_filename);
	if (scene_assets_)
	{
		mesh_instance_.set_mesh(GetMeshFromSceneAssets(scene_assets_));
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}
	
	maxfruit = 3;
	srand(time(NULL));
	//draw intial fruits
	//bananas
	for (int i = 0; i < maxfruit; i++)
	{
		float posx = -19 + 39 * (rand() / (float)RAND_MAX);
		float posy = 15 + 5 * (rand() / (float)RAND_MAX);
		//posy = 20.f;
		//posx = 1.f;
		Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 1);
		bananas.push_back(fruit);
		
	}
	//apples
	for (int i = 0; i < maxfruit; i++)
	{
		float posx = -19 + 39 * (rand() / (float)RAND_MAX);
		float posy = 15 + 5 * (rand() / (float)RAND_MAX);
		//posy = 20.f;
		//posx = 1.f;
		Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 2);
		apples.push_back(fruit);
	}
	//oranges
	for (int i = 0; i < maxfruit; i++)
	{
		float posx = -19 + 39 * (rand() / (float)RAND_MAX);
		float posy = 15 + 5 * (rand() / (float)RAND_MAX);
		//posy = 20.f;
		//posx = 1.f;
		Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 3);
		oranges.push_back(fruit);
	}

	//rotten fruit
	for (int i = 0; i < maxfruit; i++)
	{
		float posx = -19 + 39 * (rand() / (float)RAND_MAX);
		float posy = 15 + 5 * (rand() / (float)RAND_MAX);
		//posy = 20.f;
		//posx = 1.f;
		Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 4);
		rotten_fruits.push_back(fruit);
	}

	//right wall
	float posx = 20.f;
	Wall* wall = new Wall(primitive_builder_, world, posx);
	walls.push_back(wall);

	//left wall
	posx = -20.f;
	Wall* wall2 = new Wall(primitive_builder_, world, posx);
	walls.push_back(wall2);	

	g.initGround(primitive_builder_, world);
	p.initPlayer(primitive_builder_, input_manager_, world);
	InitFont();
	SetupLights();

	FlagPlayer = false;
	FlagFruit = false;
	FlagPlayerLife = false;
}

void SceneApp::CleanUp()
{
	// clean up scene assets
	delete scene_assets_;
	scene_assets_ = NULL;

	CleanUpFont();

	delete primitive_builder_;
	primitive_builder_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete world;
	world = NULL;
}

bool SceneApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;	

	int vel_iteration = 8;
	int pos_interation = 3;
	world->Step(frame_time, vel_iteration, pos_interation);

	for (int i = 0; i < walls.size(); i++)
	{
		walls[i]->update();
	}
	g.update();
	p.update();

	if (f.getnumFruits() >= 12)//while total number of fruits are greater than 15 keeping spawning more
	{
		//bananas
		if (bananas.size() < maxfruit)
		{
			float posx = -19 + 39 * (rand() / (float)RAND_MAX);
			float posy = 15 + 5 * (rand() / (float)RAND_MAX);
			//posy = 20.f;
			//posx = 1.f;
			Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 1);
			bananas.push_back(fruit);
		}

		//apples
		if (apples.size() < maxfruit)
		{
			float posx = -19 + 39 * (rand() / (float)RAND_MAX);
			float posy = 15 + 5 * (rand() / (float)RAND_MAX);
			//posy = 20.f;
			//posx = 1.f;
			Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 2);
			apples.push_back(fruit);
		}

		//oranges
		if (oranges.size() < maxfruit)
		{
			float posx = -19 + 39 * (rand() / (float)RAND_MAX);
			float posy = 15 + 5 * (rand() / (float)RAND_MAX);
			//posy = 20.f;
			//posx = 1.f;
			Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 3);
			oranges.push_back(fruit);
		}

		//Rotten fruit
		if (rotten_fruits.size() < maxfruit)
		{
			float posx = -19 + 39 * (rand() / (float)RAND_MAX);
			float posy = 15 + 5 * (rand() / (float)RAND_MAX);
			//posy = 20.f;
			//posx = 1.f;
			Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy, 4);
			rotten_fruits.push_back(fruit);
		}
		
	}
	for (int i = 0; i < bananas.size(); i++)
	{
		bananas[i]->update();
	}
	for (int i = 0; i < apples.size(); i++)
	{
		apples[i]->update();
	}
	for (int i = 0; i < oranges.size(); i++)
	{
		oranges[i]->update();
	}
	for (int i = 0; i < rotten_fruits.size(); i++)
	{
		rotten_fruits[i]->update();
	}

	GroundPlayerCollision();

	//gef::DebugOut("calling delete now!\n");
	//process list for deletion
	std::vector<Fruit*>::iterator it = BananaScheduledForRemoval.begin();	
	//delete Bananas
	if (!BananaScheduledForRemoval.empty())
	{
		for (int i = 0; i < BananaScheduledForRemoval.size(); i++)
		{
			Fruit* dyingFruit = *it;
			it[i]->collisionCheck(world);//remove from world

			///remove it from main list of fruits
			std::vector<Fruit*>::iterator it = std::find(bananas.begin(), bananas.end(), dyingFruit);
			if (it != bananas.end())
			{
				bananas.erase(it);
			}
			BananaScheduledForRemoval.clear();
		}
	}

	std::vector<Fruit*>::iterator it2 = AppleScheduledForRemoval.begin();
	//delete Apples
	if (!AppleScheduledForRemoval.empty())
	{
		for (int i = 0; i < AppleScheduledForRemoval.size(); i++)
		{
			Fruit* dyingFruit = *it2;
			it2[i]->collisionCheck(world);//remove from world

			///remove it from main list of fruits
			std::vector<Fruit*>::iterator it = std::find(apples.begin(), apples.end(), dyingFruit);
			if (it != apples.end())
			{
				apples.erase(it);
			}
			AppleScheduledForRemoval.clear();
		}
	}

	std::vector<Fruit*>::iterator it3 = OrangeScheduledForRemoval.begin();
	//delete Oranges
	if (!OrangeScheduledForRemoval.empty())
	{
		for (int i = 0; i < OrangeScheduledForRemoval.size(); i++)
		{
			Fruit* dyingFruit = *it3;
			it3[i]->collisionCheck(world);//remove from world

			///remove it from main list of fruits
			std::vector<Fruit*>::iterator it = std::find(oranges.begin(), oranges.end(), dyingFruit);
			if (it != oranges.end())
			{
				oranges.erase(it);
			}
			OrangeScheduledForRemoval.clear();
		}
	}

	std::vector<Fruit*>::iterator it4 = Rotten_FruitScheduledForRemoval.begin();
	//delete Rotten_Fruit
	if (!Rotten_FruitScheduledForRemoval.empty())
	{
		for (int i = 0; i < Rotten_FruitScheduledForRemoval.size(); i++)
		{
			Fruit* dyingFruit = *it4;
			it4[i]->collisionCheck(world);//remove from world

			///remove it from main list of fruits
			std::vector<Fruit*>::iterator it = std::find(rotten_fruits.begin(), rotten_fruits.end(), dyingFruit);
			if (it != rotten_fruits.end())
			{
				rotten_fruits.erase(it);
			}
			Rotten_FruitScheduledForRemoval.clear();
		}
	}

	//update player score
	if(FlagPlayer)
	{
		p.IncrementScore();
		FlagPlayer = false;
	}	

	//player lose life
	if (FlagPlayerLife)
	{
		p.DecrementLife();
		FlagPlayerLife = false;
	}

	//decrment number of fruits left
	if (FlagFruit)
	{
		f.decrementNumFruits();
		FlagFruit = false;
	}

	return true;
}

void SceneApp::Render()
{
	// setup camera
	// projection
	float fov = gef::DegToRad(45.0f);
	float aspect_ratio = (float)platform_.width() / (float)platform_.height();
	gef::Matrix44 projection_matrix;
	projection_matrix = platform_.PerspectiveProjectionFov(fov, aspect_ratio, 0.1f, 100.0f);
	renderer_3d_->set_projection_matrix(projection_matrix);

	// view
	gef::Vector4 camera_eye(0.0f, 10.0f, 30.0f);
	gef::Vector4 camera_lookat(0.0f, 7.0f, 0.0f);
	gef::Vector4 camera_up(0.0f, 3.0f, 0.0f);
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);
	

	// draw 3d geometry
	renderer_3d_->Begin();

	//draw player
	p.render(renderer_3d_);	
	
	//draw ground 
	g.render(renderer_3d_);

	//render walls
	for (int i = 0; i < walls.size(); i++)
	{
		walls[i]->render(renderer_3d_);
	}

	//render bananas
	renderer_3d_->set_override_material(&primitive_builder_->blue_material());
	for (int i = 0; i < bananas.size(); i++)
	{		
		bananas[i]->render(renderer_3d_);			
	}
	renderer_3d_->set_override_material(NULL);
	
	//render apples
	renderer_3d_->set_override_material(&primitive_builder_->red_material());
	for (int i = 0; i < apples.size(); i++)
	{
		//apples[i]->set_mesh(GetMeshFromSceneAssets(scene_assets_));
		apples[i]->render(renderer_3d_);
	}
	renderer_3d_->set_override_material(NULL);

	//render oranges
	renderer_3d_->set_override_material(&primitive_builder_->green_material());
	for (int i = 0; i < oranges.size(); i++)
	{
		//apples[i]->set_mesh(GetMeshFromSceneAssets(scene_assets_));
		oranges[i]->render(renderer_3d_);
	}
	renderer_3d_->set_override_material(NULL);

	//render rotten_fruits
	for (int i = 0; i < rotten_fruits.size(); i++)
	{
		//apples[i]->set_mesh(GetMeshFromSceneAssets(scene_assets_));
		rotten_fruits[i]->render(renderer_3d_);
	}

	renderer_3d_->End();

	// start drawing sprites, but don't clear the frame buffer
	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

void SceneApp::GroundPlayerCollision()
{
	// update object visuals from simulation data

	// collision detection
	// get the head of the contact list
	b2Contact* contact = world->GetContactList();
	// get contact count
	int contact_count = world->GetContactCount();
	
	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE
			Player* player = NULL;		
			Fruit* banana = NULL;
			Fruit* apple = NULL;
			Fruit* orange = NULL;
			Fruit* rotten_fruit = NULL;

			game_object* game_object1 = (game_object*)bodyA->GetUserData();
			game_object* game_object2 = (game_object*)bodyB->GetUserData();

			if (!game_object1 || !game_object2)//check for null pointers
			{
				gef::DebugOut("Null\n");
			}
			else
			{			
				//gef::DebugOut("%i\n", game_object1->type());
				//gef::DebugOut("%i\n", game_object2->type());
			}
			
			if (game_object1)
			{
				if (game_object1->type() == BANANA)
				{
					banana = (Fruit*)bodyA->GetUserData();
				}
				
				if (game_object1->type() == APPLE)
				{
					apple = (Fruit*)bodyA->GetUserData();
				}

				if (game_object1->type() == ORANGE)
				{
					orange = (Fruit*)bodyA->GetUserData();
				}

				if (game_object1->type() == ROTTEN)
				{
					rotten_fruit = (Fruit*)bodyA->GetUserData();
				}

				if (game_object1->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData();
				}
			}
			if (game_object2)
			{
				if (game_object2->type() == BANANA)
				{
					banana = (Fruit*)bodyB->GetUserData();
				}

				if (game_object2->type() == APPLE)
				{
					apple = (Fruit*)bodyB->GetUserData();
				}

				if (game_object2->type() == ORANGE)
				{
					orange = (Fruit*)bodyB->GetUserData();
				}

				if (game_object2->type() == ROTTEN)
				{
					rotten_fruit = (Fruit*)bodyB->GetUserData();
				}

				if (game_object2->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData();
				}
			}
			
			//fruit hits the player
			if(banana && player)
			{			
				//gef::DebugOut("IT! collided\n");
				BananaScheduledForRemoval.push_back(banana);
				FlagPlayer = true;
				FlagFruit = true;
			}
			
			//fruit hits the ground
			if (banana)
			{
				BananaScheduledForRemoval.push_back(banana);
				FlagFruit = true;
			}
			
			//fruit hits the player
			if (apple && player)
			{
				//gef::DebugOut("IT! collided\n");
				AppleScheduledForRemoval.push_back(apple);
				FlagPlayer = true;
				FlagFruit = true;
			}

			//fruit hits the ground
			if (apple)
			{
				AppleScheduledForRemoval.push_back(apple);
				FlagFruit = true;
			}

			//fruit hits the player
			if (orange && player)
			{
				//gef::DebugOut("IT! collided\n");
				OrangeScheduledForRemoval.push_back(orange);
				FlagPlayer = true;
				FlagFruit = true;
			}

			//fruit hits the ground
			if (orange)
			{
				OrangeScheduledForRemoval.push_back(orange);
				FlagFruit = true;
			}

			//fruit hits the player
			if (rotten_fruit && player)
			{
				//gef::DebugOut("IT! collided\n");
				Rotten_FruitScheduledForRemoval.push_back(rotten_fruit);
				FlagPlayerLife = true;
				FlagFruit = true;
			}

			//fruit hits the ground
			if (rotten_fruit)
			{
				Rotten_FruitScheduledForRemoval.push_back(rotten_fruit);
				FlagFruit = true;
			}

			//gef::DebugOut("%i\n", world->GetContactCount());
			//gef::DebugOut("%.1f\n", bodyA->GetMass());
			//gef::DebugOut("%.1f\n", bodyB->GetMass());			
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{
	if (font_)
	{
		// display frame rate
		font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 10.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i Fruits: %i", p.getScore(), f.getnumFruits());
		font_->RenderText(sprite_renderer_, gef::Vector4(10.0f, 30.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Lives: %i", p.getLife());
	}
}

void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

gef::Scene* SceneApp::LoadSceneAssets(gef::Platform& platform, const char* filename)
{
	gef::Scene* scene = new gef::Scene();

	if (scene->ReadSceneFromFile(platform, filename))
	{
		// if scene file loads successful
		// create material and mesh resources from the scene data
		scene->CreateMaterials(platform);
		scene->CreateMeshes(platform);
	}
	else
	{
		delete scene;
		scene = NULL;
	}

	return scene;
}

gef::Mesh* SceneApp::GetMeshFromSceneAssets(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	// if the scene data contains at least one mesh
	// return the first mesh
	if (scene && scene->meshes.size() > 0)
		mesh = scene->meshes.front();

	return mesh;
}
