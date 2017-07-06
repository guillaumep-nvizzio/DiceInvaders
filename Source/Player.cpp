// Player.cpp : Implementation of the player game object
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "Player.h"
#include "PlayerRocket.h"
#include "UserInterface.h"


Player::Player(Game* the_game) : GameObject(the_game)
{
	player_speed_modifier = 0;
	last_fire_time = 0.0f;
	score = 0;
	ammo = kStartingAmmo;
	health = kStartingHealth;
}

Player::~Player()
{
	Destroy();
}

bool Player::Create(const char * spite_path)
{
	bool ret = GameObject::Create(spite_path);

	assert(game != nullptr);

	// Initial player position (middle of the screen, bottom sprite is 32 pixel wide)
	position_x = game->GetScreenWidth() / 2;
	position_y = game->GetScreenHeight() - SPRITE_SIZE;

	// update the UI
	game->GetUI()->SetAmmo(ammo);
	game->GetUI()->SetScore(score);
	game->GetUI()->SetHealth(health);

	return ret;
}

void Player::Destroy()
{
	GameObject::Destroy();
}

bool Player::Update(float delta_time)
{
	// player object specific logic
	UpdateInputs(delta_time);

	UpdateRockets(delta_time);

	return GameObject::Update(delta_time);
}

void Player::UpdateRockets(float delta_time)
{
	std::vector<PlayerRocket*>::iterator it = player_rockets.begin();
	for (; it != player_rockets.end();)
	{
		PlayerRocket* rocket = (*it);
		bool is_exploded = rocket->IsExploded();

		// check is the rocket has exploded on contact of an alien, if not, update as usual
		if (is_exploded == true)
		{
			it = player_rockets.erase(it);
			delete rocket;
		}
		else
		{
			// if the update fails (rocket is out of bounds), we remove it from the list and delete the object
			bool is_valid = rocket->Update(delta_time);
			if (is_valid == false)
			{
				it = player_rockets.erase(it);
				delete rocket;
			}
			else
			{
				++it;
			}
		}
	}
}

void Player::UpdateInputs(float delta_time)
{
	IDiceInvaders::KeyStatus keys;

	game->GetSystem()->getKeyStatus(keys);
	float delta_pos = delta_time * PLAYER_SPEED + player_speed_modifier;

	// Update player position
	if (keys.right)
	{
		position_x += int(delta_pos);
	}
	else if (keys.left)
	{
		position_x -= int(delta_pos);
	}
	
	last_fire_time -= delta_time;
	
	// if the player fires his weapon, using the space key
	if (keys.fire && CanFireRocket() == true)
	{
		FireRocket();
	}

	// Limit player position to screen size
	if (position_x < 0)
	{
		position_x = 0;
	}
	if (position_x > (game->GetScreenWidth() - SPRITE_SIZE))
	{
		position_x = game->GetScreenWidth() - SPRITE_SIZE;
	}
}

/*
 This function returns true if the player can fire a rocket
 - has enough ammo
 - the fire delay has expired
*/

bool Player::CanFireRocket()
{
	if (ammo > 0 && last_fire_time <= 0.0f)
	{
		return true;
	}
	return false;
}

void Player::FireRocket()
{
	if (CanFireRocket() == true)
	{
		last_fire_time = kRocketFiringInterval;

		PlayerRocket * newRocket = new PlayerRocket(game);
		newRocket->Create(position_x, position_y);
		player_rockets.push_back(newRocket);

		// update ammos
		ammo--;
		game->GetUI()->SetAmmo(ammo);
	}
}

void Player::IncreaseScore(int value)
{
	// update score
	this->score += value;
	game->GetUI()->SetScore(score);
}

void Player::SetScore(int score)
{
	// update score
	this->score = score;
	game->GetUI()->SetScore(score);
}

void Player::SetAmmo(int ammo)
{
	// update ammos
	this->ammo = ammo;
	game->GetUI()->SetAmmo(ammo);
}

void Player::DecreaseHealth(int value)
{
	this->health -= value;
	if (this->health <= 0)
	{
		this->health = 0;
		Die();
	}
	game->GetUI()->SetHealth(health);
}

void Player::Die()
{
	game->EndGame();
}