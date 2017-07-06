#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "resource.h"
#include "DiceInvaders.h"

// A box defining the boundaries of a game object
struct Box {
	int left;
	int top;
	int right;
	int bottom;
};

/**
 * GameObject
 * Abstract class that encapsulate all game objects functionalities
 *
 **/

class GameObject
{
public:

	GameObject(class Game* the_game);
	~GameObject();

	virtual bool Create(const char * spite_path);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	void SetPositionX(int x);
	void SetPositionY(int y);

	int GetPositionX() const;
	int GetPositionY() const;

	void MoveUp(int x = 1);
	void MoveDown(int y = 1);

	bool OnScreen();
	bool Contains(int x, int y) const;
	bool Intersect(const Box & box) const;

	Box GetBox() const;

protected:

	int position_x;
	int position_y;
	int size_height;
	int size_width;

	// Sprite
	ISprite* sprite;

	class Game* game;
};


#endif //__GAMEOBJECT_H__
