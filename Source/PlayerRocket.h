#ifndef __PLAYERROCKET_H__
#define __PLAYERROCKET_H__

#include "resource.h"
#include "DiceInvaders.h"
#include "GameObject.h"

/**
 * PlayerRocket: The projectile from player ship
 *
 **/

class PlayerRocket : public GameObject
{

public:

	PlayerRocket(class Game* the_game);
	~PlayerRocket();

	virtual bool Create(int pos_x, int pos_y);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	void Move();
	void SetExploded() { exploded = true; }
	bool IsExploded() { return exploded; }

private:
	bool exploded;

	// the rocket vertical speed (pixel per update)
	static constexpr unsigned kSpeedY = 12;
};


#endif //__PLAYERROCKET_H__