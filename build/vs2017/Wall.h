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

namespace gef
{
	class Renderer3D;
}
class Wall : public game_object
{
public:
	Wall();
	Wall(PrimitiveBuilder* p, b2World* world, float posx);
	void update();
	void render(gef::Renderer3D* r);

	b2Body* m_body;
};

