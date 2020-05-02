#include "Fruit.h"

Fruit::Fruit()
{
	numofFruits = 99;
}

Fruit::Fruit(PrimitiveBuilder* p, b2World* world, float posx, float posy, int Fruit_type)
{	
	m_body = NULL;
	if (Fruit_type == 1)
	{
		set_type(BANANA);
	}
	else if(Fruit_type == 2)
	{
		set_type(APPLE);
	}
	else if(Fruit_type == 3)
	{
		set_type(ORANGE);
	}
	else
	{
		set_type(ROTTEN);
	}
	
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
	//gef::Vector4 scale(0.01f, 0.01f, 0.01f);
	gef::Vector4 scale(1.0f, 1.0f, 1.0f);

	//gef::DebugOut("Fruit body null\n");
	this->UpdateFromSimulation(m_body, scale);
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

void Fruit::decrementNumFruits()
{
	numofFruits--;

	//gef::DebugOut("Fruits: %i\n", numofFruits);
}

int Fruit::getnumFruits()
{
	return numofFruits;
}

