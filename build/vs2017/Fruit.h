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
#include <system/debug_log.h>
#include <stdio.h>   
#include <stdlib.h>    
#include <time.h> 

namespace gef
{
	class Renderer3D;
}
class Fruit : public game_object
{
public:
	Fruit();
	Fruit(PrimitiveBuilder* p, b2World* world, float posx, float posy);//creates fruit body
	void update();//updates transform of fruit
	void render(gef::Renderer3D* r);//renders fruit
	void collisionCheck(b2World* world);//delete fruit body
	void decrementNumFruits();//decrements fruit total
	int getnumFruits();//returns number of fruits
	
	
	b2Body* m_body;
	gef::Matrix44 transform;

	int numofFruits;//total number of fruits
};

