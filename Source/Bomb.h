#ifndef __BOMB_H__
#define __BOMB_H__

#include "resource.h"
#include "DiceInvaders.h"
#include "GameObject.h"

/**
 * Bomb: The projectile from aliens
 *
 **/

class Bomb : public GameObject
{

public:

	Bomb(class Game* the_game);
	~Bomb();

	virtual bool Create(int pos_x, int pos_y);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	void Move();
	void SetExploded() { exploded = true; }
	bool IsExploded() { return exploded; }

private:
	bool exploded;
	static constexpr unsigned kSpeedY = 6;
};


#endif //__BOMB_H__