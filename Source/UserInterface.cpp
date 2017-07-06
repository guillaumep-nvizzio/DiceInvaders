// UserInterface.cpp
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "UserInterface.h"



UserInterface::UserInterface(Game* the_game)
{
	state = IN_GAME;
	game = the_game;
}

UserInterface::~UserInterface()
{
	// making sure everything is clean
	Destroy();
}

bool UserInterface::Create(const char * spite_path)
{
	return true;
}

void UserInterface::Destroy()
{
}

bool UserInterface::Update(float delta_time)
{
	switch (state)
	{
	case LOBBY:
		PrintScore();
		break;
	case IN_GAME:
		PrintScore();
		PrintStats();
		PrintHealth();
		break;
	case GAME_OVER:
		PrintGameOver();
		UpdateInputs(delta_time);
		break;
	default:
		break;
	}
	return true;
}

void UserInterface::PrintStats()
{
	snprintf(text_buffer, kTextBufferSize-1, "Ammo %d", player_ammo);

	game->GetSystem()->drawText(10, 10, text_buffer);
}

void UserInterface::PrintScore()
{
	snprintf(text_buffer, kTextBufferSize - 1, "Score %d", player_score);

	game->GetSystem()->drawText(150, 10, text_buffer);
}

void UserInterface::PrintHealth()
{
	snprintf(text_buffer, kTextBufferSize - 1, "Health %d", player_health);

	game->GetSystem()->drawText(300, 10, text_buffer);
}

void UserInterface::PrintGameOver()
{
	int x = game->GetScreenWidth() / 2;
	int y = game->GetScreenHeight() / 2;

	snprintf(text_buffer, kTextBufferSize - 1, "GAME OVER !!! YOUR SCORE: %d POINTS", player_score);
	game->GetSystem()->drawText(x, y, text_buffer);

	snprintf(text_buffer, kTextBufferSize - 1, "SPACE TO CONTINUE");
	game->GetSystem()->drawText(x, y + 30, text_buffer);
}

void UserInterface::UpdateInputs(float delta_time)
{
	IDiceInvaders::KeyStatus keys;

	if (state == GAME_OVER)
	{
		game->GetSystem()->getKeyStatus(keys);

		if (keys.fire)
		{
			state = IN_GAME;
			game->StartGame();
		}
	}
}
