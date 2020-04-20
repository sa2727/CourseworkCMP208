#pragma once
#include <graphics/mesh_instance.h>
#include <box2d/Box2D.h>

enum object_type {
	GROUND, PLAYER, BANANA, APPLE
};


class game_object: public gef::MeshInstance
{
public:
	void UpdateFromSimulation(const b2Body* body);
	inline void set_type(object_type type) { type_ = type; }
	inline object_type type() { return type_; }

private:
	object_type type_;
};

