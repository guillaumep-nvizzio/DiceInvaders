// Alien.cpp : Implementation of the alien game object
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "Alien.h"
#include "Bomb.h"


Alien::Alien(Game* the_game) : GameObject(the_game)
{
	is_dead = false;
}

Alien::~Alien()
{
	Destroy();
}

bool Alien::Create(AlienType alienType, int pos_x, int pos_Y)
{
	bool ret = false;

	type = alienType;

	switch (type)
	{
	case AlienType::RED:
		ret = GameObject::Create(ALIEN1_SPRITE_PATH);
		break;
	case AlienType::GREEN:
		ret = GameObject::Create(ALIEN2_SPRITE_PATH);
		break;
	default:
		ret = GameObject::Create(ALIEN2_SPRITE_PATH);
		break;
	}
	
	SetPositionX(pos_x);
	SetPositionY(pos_Y);

	return ret;
}

void Alien::Destroy()
{
	GameObject::Destroy();
}

bool Alien::Update(float delta_time)
{
	// ...

	return GameObject::Update(delta_time);
}


Bomb* Alien::DropBomb()
{
	Bomb * newBomb = new Bomb(game);
	newBomb->Create(position_x, position_y);
	return newBomb;
}
