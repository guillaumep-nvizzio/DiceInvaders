#ifndef __ALIENFORMATION_H__
#define __ALIENFORMATION_H__

#include "resource.h"
#include "DiceInvaders.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <vector>


/**
 * AlienFormation: Represents the entire Space Invader formation.
 * Coordinates movements and firing of all Space Invaders.
 *
 **/

typedef std::vector<class Bomb *> TBombs;
typedef std::vector<class Alien *> TAliens;

class AlienFormation
{
public:

	AlienFormation(class Game* the_game);
	~AlienFormation();

	bool Create(int num_rows, int num_columns);
	void Destroy();
	void Reset();
	void Update(float delta_time);

	void UpdateRocketCollisions(TRockets rockets);
	void UpdateBombs(float delta_time);

	int AliensCount();

private:

	// formation movement
	void Move();			// sideways
	void Down();			// move down
	void ShiftDirection();	// shift movement direction (sideways)

	// Firing
	void IncreaseAttackRate(int amount);
	bool ShouldDropBomb();
	
	// Getting aliens objects
	class Alien * GetOutermostAlien();	// return the alien at the edge of the formation on the Y axis, based on direction, if more than one alien has the same position on the y axis, the first one will be returned.
	class Alien * GetRandomAlien();		// return a random alien in the formation
	class Alien * GetLowestAlien();		// return the alien with the lowest altitude, closest to the player, if more than one alien has the same altitude, the first one will be returned.

	TAliens aliens;
	TBombs bombs;

	// Where the formation starts on screen
	static constexpr unsigned kStartPosX = 0;
	static constexpr unsigned kStartPosY = 0;

	// The space between each invader in the formation
	static constexpr unsigned kGapX = 15;
	static constexpr unsigned kGapY = 5;

	// The speed of the formation on each axis
	static constexpr unsigned kSpeedX = 2;
	static constexpr unsigned kSpeedY = 12;

	static constexpr int kBombRatioStart = 100;

	// damage from falling bomb (3 times and death as per assignment)
	static constexpr int kBombDamage = 35;

	// damage from alien collision (3 times and death as per assignment)
	static constexpr int kAlienCollisionDamage = 35;

	// flag for move direction of the formation (true: right, false: left)
	bool move_direction;

	int formation_speed_modifier;

	// the current attack rate at which aliens are dropping bombs
	int bomb_ratio;

	class Game* game;
};


#endif //__ALIENFORMATION_H__