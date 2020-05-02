#include "Wall.h"

Wall::Wall()
{

}

Wall::Wall(PrimitiveBuilder* p, b2World* world, float posx)
{
	set_type(WALL);

	//mesh for static player
	set_mesh(p->GetDefaultCubeMesh());

	//physics body
	static b2BodyDef static_body_def;
	static_body_def.type = b2_staticBody;
	static_body_def.position = b2Vec2(posx, 0.0f);

	m_body = world->CreateBody(&static_body_def);

	m_body->SetUserData(this);

	//create shape
	b2PolygonShape static_shape;
	static_shape.SetAsBox(0.5f, 0.5f);

	//create fixture
	b2FixtureDef static_fixture_def;
	static_fixture_def.density = 1.0f;
	static_fixture_def.shape = &static_shape;
	static_fixture_def.friction = 1.0f;
	static_fixture_def.restitution = 0.5f;

	//bind fixture
	m_body->CreateFixture(&static_fixture_def);
}

void Wall::update()
{
	//create static body transform
	gef::Vector4 scale(1.0f, 5.0f, 5.0f);
	this->UpdateFromSimulation(m_body, scale);
}

void Wall::render(gef::Renderer3D* r)
{
	r->DrawMesh(*this);
}
