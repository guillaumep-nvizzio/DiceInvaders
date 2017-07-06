#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "resource.h"
#include "DiceInvaders.h"
#include "GameObject.h"

#include <iostream>
#include <vector>

/**
 * Player: represent the player's ship
 * 
 **/

typedef std::vector<class PlayerRocket *> TRockets;

class Player : public GameObject
{
public:

	Player(class Game* the_game);
	~Player();

	virtual bool Create(const char * spite_path);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	TRockets GetPlayerRockets() { return player_rockets;  }

	void SetScore(int score);
	void IncreaseScore(int value);
	void SetAmmo(int ammo);
	void DecreaseHealth(int value);

	void Die();

private:
	int player_speed_modifier;

	// Update the player inputs and updating the position
	void UpdateInputs(float delta_time);

	void UpdateRockets(float delta_time);

	void FireRocket();
	bool CanFireRocket();

	TRockets player_rockets;

	static constexpr float kRocketFiringInterval = 0.2f;
	static constexpr unsigned kStartingAmmo = 100;
	static constexpr unsigned kStartingHealth = 100;
	float last_fire_time;

	int ammo;
	int score;
	int health;
};


#endif //__PLAYER_H__