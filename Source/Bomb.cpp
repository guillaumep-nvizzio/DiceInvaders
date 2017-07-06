// Bomb.cpp : Implementation of the bomb game object
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "Bomb.h"


Bomb::Bomb(Game* the_game) : GameObject(the_game)
{
	exploded = false;
}

Bomb::~Bomb()
{
	Destroy();
}

bool Bomb::Create(int pos_x, int pos_y)
{
	bool ret = false;

	ret = GameObject::Create(BOMB_SPRITE_PATH);

	SetPositionX(pos_x);
	SetPositionY(pos_y);

	return ret;
}

void Bomb::Destroy()
{
	GameObject::Destroy();
}

bool Bomb::Update(float delta_time)
{
	// the bomb moves at every update
	Move();

	if (OnScreen() == false)
	{
		return false;
	}

	if (exploded == true)
	{
		return false;
	}

	return GameObject::Update(delta_time);
}

void Bomb::Move()
{
	// moving the bomb down at the pre-determined speed
	SetPositionY(position_y + kSpeedY);
}