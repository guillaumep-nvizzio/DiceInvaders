// GameObject.cpp : Base class for all game objects
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "GameObject.h"

#include <algorithm>
#include <iostream>


GameObject::GameObject(Game* the_game)
{
	game = the_game;
	sprite = nullptr;

	size_height = SPRITE_SIZE;
	size_width = SPRITE_SIZE;
}

GameObject::~GameObject()
{
	// making sure everything is clean
	Destroy();
}

bool GameObject::Create(const char * spite_path)
{
	if (spite_path != nullptr)
	{
		sprite = game->GetSystem()->createSprite(spite_path);
	}

	position_x = 0;
	position_y = 0;

	return true;
}

void GameObject::Destroy()
{
	if (sprite != nullptr)
	{
		sprite->destroy();
		sprite = nullptr;
	}
}

bool GameObject::Update(float delta_time)
{
	// draw the sprite
	if (sprite != nullptr)
	{
		sprite->draw(position_x, position_y);
	}
	
	return true;
}

void GameObject::SetPositionX(int x)
{
	position_x = x;
}

void GameObject::SetPositionY(int y)
{
	position_y = y;
}

int GameObject::GetPositionX() const
{
	return position_x;
}

int GameObject::GetPositionY() const
{
	return position_y;
}

void GameObject::MoveUp(int x)
{
	position_x += x;
}

void GameObject::MoveDown(int y)
{
	position_y -= y;
}

/**
* Return true if the game object is visible on screen
*/
bool GameObject::OnScreen()
{
	int minX = 0;
	int maxX = game->GetScreenWidth();
	int minY = 0;
	int maxY = game->GetScreenHeight();

	// check if the game object is on screen
	return (position_x >= minX) && (position_x < maxX) && (position_y >= minY) && (position_y < maxY);
}

/**
* Return true if the coordinates passed in are contained in the game object
*/
bool GameObject::Contains(int x, int y) const
{
	// Compute the real min and max of the object on both axes, i'm using ((std::min)) to bypass windows macros - evil macros
	int minX = (std::min)(position_x, (position_x + size_width));
	int maxX = (std::max)(position_x, (position_x + size_width));
	int minY = (std::min)(position_y, (position_y + size_height));
	int maxY = (std::max)(position_y, (position_y + size_height));

	return (x >= minX) && (x < maxX) && (y >= minY) && (y < maxY);
}

/**
* Return true if this game object touches the object passed as argument
*/
bool GameObject::Intersect(const Box & box) const
{
	// check for all the points of the game object if they are contained in this game object
	return (Contains(box.left, box.bottom) || Contains(box.left, box.top) || Contains(box.right, box.bottom) || Contains(box.right, box.top));
}

/**
* Returns the coordinates of the box containing the game object
*/
Box GameObject::GetBox() const
{
	Box box;
	box.left = (std::min)(position_x, (position_x + size_width));
	box.right = (std::max)(position_x, (position_x + size_width));
	box.bottom = (std::min)(position_y, (position_y + size_height));
	box.top = (std::max)(position_y, (position_y + size_height));

	return box;
}