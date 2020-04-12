#pragma once
#include <system/application.h>
#include <maths/vector2.h>
#include <maths/vector4.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include "graphics/scene.h"
#include "box2d/box2d.h"
#include "game_object.h"
#include "graphics/renderer_3d.h"
#include <input/keyboard.h>
#include <input/input_manager.h>
#include <system/debug_log.h>

namespace gef
{
	class Renderer3D;
	class InputManger;
}
class Player : public game_object
{
public:
	Player();
	void initPlayer(PrimitiveBuilder* p, gef::InputManager* i, b2World* world);
	void update();
	void move();//Player Controls
	void render(gef::Renderer3D* r, PrimitiveBuilder* p);
	void IncrementScore();
	int getScore();

	b2Body* m_body;
	gef::Matrix44 transform;
	gef::InputManager* input_manager;

	int score;

};

