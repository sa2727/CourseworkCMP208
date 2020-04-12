#include "Ground.h"

Ground::Ground()
{
	set_type(GROUND);
}

void Ground::initGround(PrimitiveBuilder* p, b2World* world)
{	
	//mesh for static player
	set_mesh(p->GetDefaultCubeMesh());

	//physics body
	static b2BodyDef static_body_def;
	static_body_def.type = b2_staticBody;
	static_body_def.position = b2Vec2(0.f, -2.f);

	m_body = world->CreateBody(&static_body_def);

	m_body->SetUserData(this);

	//create shape
	b2PolygonShape static_shape;
	static_shape.SetAsBox(20.f, 0.5f);

	//create fixture
	b2FixtureDef static_fixture_def;
	static_fixture_def.density = 1.0f;
	static_fixture_def.shape = &static_shape;
	static_fixture_def.friction = 1.0f;

	//bind fixture
	m_body->CreateFixture(&static_fixture_def);
}

void Ground::update()
{
	//create static body transform
	gef::Vector4 ground_pos(m_body->GetPosition().x, m_body->GetPosition().y, 0);
	transform.SetIdentity();
	transform.Scale(gef::Vector4(40.0f, 1.0f, 10.0f));
	transform.SetTranslation(ground_pos);
	set_transform(transform);
}

void Ground::render(gef::Renderer3D* r)
{
	r->DrawMesh(*this);
}
