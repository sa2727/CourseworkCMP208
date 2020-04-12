#include "Fruit.h"

Fruit::Fruit()
{
}

Fruit::Fruit(PrimitiveBuilder* p, b2World* world, float posx, float posy)
{
	m_body = NULL;
	set_type(FRUIT);
	score = 0;

	set_mesh(p->GetDefaultCubeMesh());
	
	//physics body
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position = b2Vec2(posx, posy);

	// set mass data
	b2MassData mData;
	mData.center = b2Vec2(0.0f, 0.0f);
	mData.mass = 1.0f;
	mData.I = 1.0f;
	
	m_body = world->CreateBody(&bdef);	 

	m_body->SetMassData(&mData);

	m_body->SetUserData(this);

	//shape
	b2PolygonShape shape;
	shape.SetAsBox(0.5f, 0.5f);

	//fixture
	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.density = 1;
	m_body->CreateFixture(&fdef);

	m_body->SetLinearDamping(3.f);
}

void Fruit::update()
{
	if (m_body != NULL)
	{
		gef::Vector4 pos(m_body->GetPosition().x, m_body->GetPosition().y, 0.f);
		transform.SetIdentity();
		transform.Scale(gef::Vector4(2.0f, 2.0f, 0.f));
		transform.SetTranslation(pos);
		set_transform(transform);
	}
	
	//gef::DebugOut("Fruit body null\n");
	this->UpdateFromSimulation(m_body);
}

void Fruit::render(gef::Renderer3D* r)
{
	if (m_body != NULL)
	{
		r->DrawMesh(*this);
	}
	
	//gef::DebugOut("no render\n");
}

void Fruit::collisionCheck(b2World* world)
{
	world->DestroyBody(m_body);
	m_body = NULL;
}
