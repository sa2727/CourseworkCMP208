#include "Player.h"

Player::Player()
{
	set_type(PLAYER);
	score = 0;
}

void Player::initPlayer(PrimitiveBuilder* pb, gef::InputManager* i, b2World* world)
{
	//input manager
	input_manager = i;

	set_mesh(pb->GetDefaultCubeMesh());

	//physics body
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;
	bdef.position = b2Vec2(1.0f, 2.0f);

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
	fdef.density = 1.f;

	m_body->CreateFixture(&fdef);
	
}

void Player::update()
{
	gef::Vector4 pos(m_body->GetPosition().x, m_body->GetPosition().y, 0.f);
	transform.SetIdentity();
	transform.Scale(gef::Vector4(2.0f, 2.0f, 0.f));
	transform.SetTranslation(pos);
	set_transform(transform);

	this->UpdateFromSimulation(m_body);

	input_manager->Update();
	move();
	
}

void Player::move()
{
	if (input_manager)
	{
		//input_manager->Update();
		
		gef::Keyboard* keyboard = input_manager->keyboard();

		if (keyboard)
		{
			if (keyboard->IsKeyDown(keyboard->KC_D))
			{
				m_body->ApplyForceToCenter(b2Vec2(10.f, 0.0f), true);
			}

			if (keyboard->IsKeyDown(keyboard->KC_A))
			{
				m_body->ApplyForceToCenter(b2Vec2(-10.f, 0.0f), true);
			}

		}
	}
}

void Player::render(gef::Renderer3D* r,PrimitiveBuilder* pb)
{
	r->DrawMesh(*this);
	r->set_override_material(NULL);
}

void Player::IncrementScore()
{	
	score++;
	
	gef::DebugOut("Players score: %i\n", score);
}

int Player::getScore()
{
	return score;
}
