// PlayerRocket.cpp : Implementation of the player rocket game object
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "PlayerRocket.h"


PlayerRocket::PlayerRocket(Game* the_game) : GameObject(the_game)
{
	exploded = false;
}

PlayerRocket::~PlayerRocket()
{
	Destroy();
}

bool PlayerRocket::Create(int pos_x, int pos_y)
{
	bool ret = false;

	ret = GameObject::Create(ROCKET_SPRITE_PATH);

	SetPositionX(pos_x);
	SetPositionY(pos_y);

	return ret;
}

void PlayerRocket::Destroy()
{
	GameObject::Destroy();
}

bool PlayerRocket::Update(float delta_time)
{
	// the rocket moves at every update
	Move();

	// check if the rocket is out of the screen, if so it is deleted.
	if (OnScreen() == false)
	{
		return false;
	}

	return GameObject::Update(delta_time);
}

void PlayerRocket::Move()
{
	// moving the rocket up at the pre-determined speed
	SetPositionY(position_y - kSpeedY);
}