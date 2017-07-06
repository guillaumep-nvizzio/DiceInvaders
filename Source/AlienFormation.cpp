// AlienFormation.cpp : Implementation of the alien group: movement logic, firing, etc..
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "AlienFormation.h"
#include "Alien.h"
#include "DiceLog.h"
#include "Player.h"
#include "PlayerRocket.h"
#include "Bomb.h"

AlienFormation::AlienFormation(Game* the_game)
{
	game = the_game;
	move_direction = false;
	bomb_ratio = kBombRatioStart;
	formation_speed_modifier = 0;
}

AlienFormation::~AlienFormation()
{
}

bool AlienFormation::Create(int num_rows, int num_columns)
{
	int pos_x = kStartPosX;
	int pos_y = kStartPosY;
	for (int x = 0; x < num_columns ; x++)
	{
		for (int y = 0; y < num_rows; y++)
		{
			Alien *newAlien = new Alien(game);
			newAlien->Create(Alien::RED, pos_x, pos_y);

			aliens.push_back(newAlien);

			pos_y += (SPRITE_SIZE + kGapY);
		}

		pos_y = kStartPosY;
		pos_x += (SPRITE_SIZE + kGapX);
	}

	return true;
}

void AlienFormation::Destroy()
{
	// Destroy invaders
	for (auto& alien : aliens)
	{
		delete alien;
	}
	
	aliens.clear();
}

void AlienFormation::Reset()
{
	move_direction = false;
	bomb_ratio = kBombRatioStart;
	formation_speed_modifier = 0;
}

int AlienFormation::AliensCount()
{
	return aliens.size();
}

/**
* AlienFormation Update: 
*   - manages alien formation  movement
*   - alien collisions, bombs and rocket collisions
*
*/
void AlienFormation::Update(float delta_time)
{
	const Player *local_player = game->GetPlayer();

	// Update invaders
	std::vector<Alien*>::iterator it = aliens.begin();
	for (; it != aliens.end();)
	{
		Alien* alien = (*it);

		// check if an alien touched the player
		if(alien->Intersect(local_player->GetBox()))
		{
			// kill the alien
			alien->Die();

			// apply damage to the player
			game->GetPlayer()->DecreaseHealth(kAlienCollisionDamage);
		}

		if (alien->IsDead() == false)
		{
			alien->Update(delta_time);
			it++;
		}
		else
		{
			it = aliens.erase(it);
			delete alien;
		}
	}

	// move the aliens sideways
	Move();
	
	// Alien will drop a bomb if required
	if (ShouldDropBomb() == true)
	{
		Alien *attacking_alien = GetRandomAlien();
		if (attacking_alien != nullptr)
		{
			Bomb * bomb = attacking_alien->DropBomb();
			bombs.push_back(bomb);
		}
	}

	// Update the alien formation direction
	Alien * outermost_alien = GetOutermostAlien();
	static int down_count = 0;		// calibration: make aliens go faster after a few times they go down.

	if (outermost_alien != nullptr)
	{
		int pos_x = outermost_alien->GetPositionX();

		if( ( move_direction == true && pos_x >= (game->GetScreenWidth() - SPRITE_SIZE)) ||
			( move_direction == false && pos_x <= 0) )
		{
			// change movement direction
			ShiftDirection();

			// shift the formation down
			Down();
			down_count++;

			// increase the chance that one alien will drop a bomb
			IncreaseAttackRate(5);

			// increase the formation speed
			if(down_count > 3)
			{
				formation_speed_modifier += 1;
				down_count = 0;
			}
			
		}
	}

	// check if the lowest alien has touched the ground
	Alien * lowest_alien = GetLowestAlien();
	if (lowest_alien != nullptr && (lowest_alien->GetPositionY() + SPRITE_SIZE > game->GetScreenHeight()))
	{
		DICE_LOG_INFO("AlienFormation: Alien OFF Screen! Game Over");
		game->EndGame();
	}

}

void AlienFormation::UpdateBombs(float delta_time)
{
	std::vector<Bomb*>::iterator it = bombs.begin();
	for (; it != bombs.end();)
	{
		Bomb* bomb = (*it);

		// if a bomb is exploded, or is off screen, the update will return false, making the bomb invalid and we will delete it.
		bool is_valid = bomb->Update(delta_time);
		if (is_valid == true)
		{
			// the bomb position is in the middle of the sprite rectangle
			int bomb_pos_x = bomb->GetPositionX() + (SPRITE_SIZE / 2);
			int bomb_pos_y = bomb->GetPositionY() + (SPRITE_SIZE / 2);

			bool does_contact = game->GetPlayer()->Contains(bomb_pos_x, bomb_pos_y);
			if (does_contact == true)
			{
				DICE_LOG_INFO("AlienFormation: Bomb hit a player!");
				bomb->SetExploded();
				game->GetPlayer()->DecreaseHealth(kBombDamage);
			}
			it++;
		}
		else
		{
			it = bombs.erase(it);
			delete bomb;
		}
	}
}

/**
 * Move the alien formation sideways, depending on the move_direction and speed
 */
void AlienFormation::Move()
{
	for (auto& alien : aliens)
	{
		int pos_x = alien->GetPositionX();

		if (move_direction == true)
		{
			alien->SetPositionX(pos_x + kSpeedX + formation_speed_modifier);
		}
		else
		{
			alien->SetPositionX(pos_x - kSpeedX - formation_speed_modifier);
		}
	}
}

/**
 * Move the alien formation down
 */
void AlienFormation::Down()
{
	for (auto& alien : aliens)
	{
		int pos_y = alien->GetPositionY();
		alien->SetPositionY(pos_y + kSpeedY);
	}
}

/**
 * Return a random alien in the formation, returns nullptr if the formation is empty
 */
Alien * AlienFormation::GetRandomAlien()
{
	if (aliens.size() > 0)
	{
		int randIndex = rand() % aliens.size();
		return aliens[randIndex];
	}
	return nullptr;
}

/**
 * Return the alien at the outermost position of the formation, depending on formation movement
 * if moving right, the alien at the extreme right will be returned, same for the left side if moving left.
 */
Alien * AlienFormation::GetOutermostAlien()
{
	int maxmin_x = 0;
	Alien *outermost_alien = nullptr;

	for (auto& alien : aliens)
	{
		// set the first one
		if (outermost_alien == nullptr)
		{
			outermost_alien = alien;
			maxmin_x = outermost_alien->GetPositionX();
		}

		int pos_x = alien->GetPositionX();

		// if the formation is moving right, get the alien with the biggest position x value
		if (move_direction == true)
		{
			if (pos_x > maxmin_x)
			{
				outermost_alien = alien;
				maxmin_x = pos_x;
			}
		}
		// formation moving left, get the alien with the smallest position x value
		else
		{
			if (pos_x < maxmin_x)
			{
				outermost_alien = alien;
				maxmin_x = pos_x;
			}
		}
	}

	return outermost_alien;
}

/**
* Return the alien with the lowest altitude, closest to the player, if more than one alien has the same altitude, the first one will be returned.
*/
Alien * AlienFormation::GetLowestAlien()
{
	int max_y = 0;
	Alien *lowest_alien = nullptr;

	for (auto& alien : aliens)
	{
		// set the first one
		if (lowest_alien == nullptr)
		{
			lowest_alien = alien;
			max_y = lowest_alien->GetPositionY();
		}

		int pos_y = alien->GetPositionY();

		if (pos_y > max_y)
		{
			lowest_alien = alien;
			max_y = pos_y;
		}
	
	}

	return lowest_alien;
}


/**
* Change the direction of the alien formation
*/
void AlienFormation::ShiftDirection()
{
	DICE_LOG_INFO("AlienFormation: Shift Direction!")
	move_direction = !move_direction;
}

/**
 * Increase the attack rate by the provided amount, with a limit of 10
 */
void AlienFormation::IncreaseAttackRate(int amount)
{
	bomb_ratio -= amount;

	// cap the bomb ratio to 1 in 10
	if (bomb_ratio < 10)
		bomb_ratio = 10;
}

/**
 * If an alien drops a bomb or not, 1 / bomb_ratio chance of returning true
 */
bool AlienFormation::ShouldDropBomb()
{
	bool should_drop = ((rand() % bomb_ratio) + 1) == 1;

	if (should_drop == true)
	{
		DICE_LOG_INFO("AlienFormation: Alien will drop a bomb");
	}

	return should_drop;
}

/**
 * Loop in the alien formation and check for every rockets if they collided, if so, kill the alien, destroy 
 * the rocket and increase the player score.
 **/
void AlienFormation::UpdateRocketCollisions(TRockets rockets)
{
	for (auto& alien : aliens)
	{
		for (auto& rocket : rockets)
		{
			// in order to detect if a rocket is in contact with an alien, we check if the position of the rocket is located in the alien rectangle
			// the rocket position is in the middle of the rectangle
			int rocket_pos_x = rocket->GetPositionX() + (SPRITE_SIZE / 2);
			int rocket_pos_y = rocket->GetPositionY() + (SPRITE_SIZE / 2);
			bool does_contact = alien->Contains(rocket_pos_x, rocket_pos_y);
			if (does_contact == true)
			{
				DICE_LOG_INFO("AlienFormation: Rocket hit an alien!");
				rocket->SetExploded();
				alien->Die();

				game->GetPlayer()->IncreaseScore(10);
			}
		}
	}

}