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
	const char* scene_asset_filename = "car.scn";
	scene_assets_ = LoadSceneAssets(platform_, scene_asset_filename);
	if (scene_assets_)
	{
		mesh_instance_.set_mesh(GetMeshFromSceneAssets(scene_assets_));
	}
	else
	{
		gef::DebugOut("Scene file %s failed to load\n", scene_asset_filename);
	}
	
	for (int i = 0; i < 5; i++)
	{
		float posx = 1 + 20 * (rand() / (float)RAND_MAX);
		float posy = 10 + 20 * (rand() / (float)RAND_MAX);
		//posy = 15.f;
		//posx = 1.f;
		Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy);
		fruits.push_back(fruit);
	}

	g.initGround(primitive_builder_, world);
	p.initPlayer(primitive_builder_, input_manager_, world);
	InitFont();
	SetupLights();

	FlagPlayer = false;
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

	g.update();
	p.update();

	//always have 5 fruits on screen
	if (fruits.size() <= 5)
	{
		float posx = 1 + 20 * (rand() / (float)RAND_MAX);
		float posy = 10 + 10 * (rand() / (float)RAND_MAX);
		//posy = 15.f;
		//posx = 1.f;
		Fruit* fruit = new Fruit(primitive_builder_, world, posx, posy);
		fruits.push_back(fruit);
	}	

	for (int i = 0; i < fruits.size(); i++)
	{
		fruits[i]->update();
	}

	GroundPlayerCollision();

	//gef::DebugOut("calling delete now!\n");
	//process list for deletion
	std::vector<Fruit*>::iterator it = FruitScheduledForRemoval.begin();
	std::vector<Fruit*>::iterator end = FruitScheduledForRemoval.end();
	
	//delete Fruits
	if (!FruitScheduledForRemoval.empty())
	{
		for (int i = 0; i < FruitScheduledForRemoval.size(); i++)
		{
			Fruit* dyingFruit = *it;
			it[i]->collisionCheck(world);

			///remove it from main list of fruits
			std::vector<Fruit*>::iterator it = std::find(fruits.begin(), fruits.end(), dyingFruit);
			if (it != fruits.end())
			{
				fruits.erase(it);
			}
			FruitScheduledForRemoval.clear();
		}
	}

	if(FlagPlayer)
	{
		//update player score
		p.IncrementScore();

		FlagPlayer = false;
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
	gef::Vector4 camera_eye(0.0f, 7.0f, 50.0f);
	gef::Vector4 camera_lookat(0.0f, 0.0f, 0.0f);
	gef::Vector4 camera_up(0.0f, 1.0f, 0.0f);
	gef::Matrix44 view_matrix;
	view_matrix.LookAt(camera_eye, camera_lookat, camera_up);
	renderer_3d_->set_view_matrix(view_matrix);
	
	// draw 3d geometry
	renderer_3d_->Begin();

	//draw player
	p.render(renderer_3d_, primitive_builder_);
	
	//draw ground 
	g.render(renderer_3d_);

	//draw fruit 
	renderer_3d_->set_override_material(&primitive_builder_->green_material());
	for (int i = 0; i < fruits.size(); i++)
	{
		fruits[i]->render(renderer_3d_);
	}
	renderer_3d_->set_override_material(NULL);


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
			Fruit* fruit = NULL;

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
				if (game_object1->type() == FRUIT)
				{
					fruit = (Fruit*)bodyA->GetUserData();
				}
				
				if (game_object1->type() == PLAYER)
				{
					player = (Player*)bodyA->GetUserData();
				}
			}
			if (game_object2)
			{
				if (game_object2->type() == FRUIT)
				{
					fruit = (Fruit*)bodyB->GetUserData();
				}

				if (game_object2->type() == PLAYER)
				{
					player = (Player*)bodyB->GetUserData();
				}
			}
			
			//fruit hits the player
			if(fruit && player)
			{			
				//gef::DebugOut("IT! collided\n");
				FruitScheduledForRemoval.push_back(fruit);
				FlagPlayer = true;
			}
			
			//fruit hits the ground
			if (fruit)
			{
				FruitScheduledForRemoval.push_back(fruit);
			}
			
			//gef::DebugOut("%i\n", world->GetContactCount());
			//gef::DebugOut("%.1f\n", bodyA->GetMass());
			//gef::DebugOut("%.1f\n", bodyB->GetMass());
			
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}
void SceneApp::ProcessTouchInput()
{
	const gef::TouchInputManager* touch_input = input_manager_->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{			

			//gef::DebugOut("x norm: %f\n y norm: %f\n", aimDirNorm.x, aimDirNorm.y);

			// if active touch id is -1, then we are not currently processing a touch
			if (active_touch_id_ == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					active_touch_id_ = touch->id;

					// do any processing for a new touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;

					//save where this position is as start_touch_position
					first_touch = touch_position_;
				}
			}
			else if (active_touch_id_ == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// we're just going to record the position of the touch
					touch_position_ = touch->position;
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything here apart from resetting the active touch id
					active_touch_id_ = -1;

					
				}
			}
		}
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
