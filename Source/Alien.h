#ifndef __ALIEN_H__
#define __ALIEN_H__

#include "resource.h"
#include "DiceInvaders.h"
#include "GameObject.h"

/**
 * Alien: An individual Space Invader
 *
 **/

class Alien : public GameObject
{
	// This is up here because there is 2 sprites and type of aliens.
	// The different types of Invaders (green and red ones. they could be of different health)
public: enum AlienType { RED, GREEN };

public:

	Alien(class Game* the_game);
	~Alien();

	virtual bool Create(AlienType type, int pos_x, int pos_Y);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	class Bomb* DropBomb();

	void Die() { is_dead = true; }
	bool IsDead() { return is_dead; }
private:
	bool is_dead;
	AlienType type;

};


#endif //__ALIEN_H__