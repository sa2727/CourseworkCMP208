#include "Fruit.h"

Fruit::Fruit()
{
	set_type(FRUIT);
}

Fruit::~Fruit()
{
	//m_body->GetWorld()->DestroyBody(m_body);
}

void Fruit::initFruit(PrimitiveBuilder* p, b2World* world)
{
	set_mesh(p->GetDefaultCubeMesh());

	//physics body
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position = b2Vec2(8.0f, 20.0f);

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
}

void Fruit::update()
{
	if (m_body == NULL)
	{
		gef::DebugOut("Fruit body null\n");
	}
	else
	{
		gef::Vector4 pos(m_body->GetPosition().x, m_body->GetPosition().y, 0.f);
		transform.SetIdentity();
		transform.Scale(gef::Vector4(2.0f, 2.0f, 0.f));
		transform.SetTranslation(pos);
		set_transform(transform);
	}
	

	this->UpdateFromSimulation(m_body);
}

void Fruit::render(gef::Renderer3D* r)
{
	if (m_body != NULL)
	{
		r->DrawMesh(*this);
	}
	
	gef::DebugOut("no render\n");
}

void Fruit::CollisionResponse(b2World* world)
{
	if (m_body)
	{
		world->DestroyBody(m_body);
		m_body = NULL;
	}
}
