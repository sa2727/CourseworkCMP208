#ifndef _SCENE_APP_H
#define _SCENE_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include "graphics/scene.h"
#include "box2d/box2d.h"
#include "game_object.h"
#include "Fruit.h"
#include "Player.h"
#include "Ground.h"
#include "Wall.h"
#include <vector>

// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class InputManager;
	class Renderer3D;
}

class SceneApp : public gef::Application
{
public:
	SceneApp(gef::Platform& platform);
	void Init();
	void CleanUp();
	bool Update(float frame_time);
	void Render();
	void GroundPlayerCollision();
private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();
	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;
	gef::Renderer3D* renderer_3d_;
	
	PrimitiveBuilder* primitive_builder_;
	gef::InputManager* input_manager_;

	Int32 active_touch_id_;
	gef::Vector2 touch_position_;
	gef::Vector2 first_touch;

	b2World* world;

	gef::Scene* scene_assets_;
	gef::MeshInstance mesh_instance_;
	
	float fps_;

	//fruit variables
	Fruit f;

	std::vector<Fruit*> bananas;
	std::vector<Fruit*> apples;
	std::vector<Fruit*> oranges;
	std::vector<Fruit*> rotten_fruits;

	std::vector<Fruit*> BananaScheduledForRemoval;
	std::vector<Fruit*> AppleScheduledForRemoval;
	std::vector<Fruit*> OrangeScheduledForRemoval;
	std::vector<Fruit*> Rotten_FruitScheduledForRemoval;
	

	bool FlagFruit;
	int maxfruit;

	//Player variables
	Player p;
	bool FlagPlayer;//fruits hits player
	bool FlagPlayerLife;//player loses a life

	//ground variables
	Ground g;

	//wall variables
	Wall w;
	std::vector<Wall*> walls;
};
#endif // _SCENE_APP_H