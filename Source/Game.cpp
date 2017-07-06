// Game.cpp : Main entry point for the game
//

#include "stdafx.h"

#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "DiceLog.h"

#include "Player.h"
#include "AlienFormation.h"
#include "UserInterface.h"

Game::Game()
{
	system = nullptr;
	playing = false;
	ending_game = false;
}


Game::~Game()
{
	system = nullptr;
}


bool Game::Initialize(IDiceInvaders* sys, int width, int height)
{
	if (sys == nullptr)
	{
		DICE_LOG_ERROR("Cannot initialize Game: invalid system pointer");
		return false;
	}

	game_ui = new UserInterface(this);

	system = sys;

	// Initialize the Dice library
	system->init(width, height);

	screen_width = width;
	screen_height = height;

	return true;
}

void Game::Deinitialize()
{
	// destroy the game objects (player, aliens, ...)
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}

	if(system != nullptr)
	{
		system->destroy();
	}
	
	system = nullptr;
	playing = false;
	ending_game = false;
}

void Game::StartGame()
{
	// Create the player, and aliens...
	CreatePlayer();
	CreateAliensFormation();
	playing = true;
	ending_game = false;
}

/**
*
*/
void Game::EndGame()
{
	ending_game = true;
}

void Game::Cleanup()
{
	// delete the player and the aliens formation

	delete player;
	player = nullptr;

	delete aliens;
	aliens = nullptr;
	
	game_ui->SetState(UserInterface::MenuState::GAME_OVER);

	playing = false;
}

void Game::CreatePlayer()
{
	player = new Player(this);
	assert(player != nullptr);

	player->Create(PLAYER_SPRITE_PATH);
}


void Game::CreateAliensFormation()
{
	aliens = new AlienFormation(this);
	assert(aliens != nullptr);

	aliens->Create(5, 5);
}


bool Game::Update(float delta_time)
{
	if (system->update() == false)
	{
		DICE_LOG_ERROR("Error on System Update");
		return false;
	}

	UpdateUI(delta_time);

	if (playing == true)
	{
		UpdateGameObjects(delta_time);
	}
	
	if (ending_game == true)
	{
		// system->update() will clears the screen and draws all sprites and texts which have been drawn since the last update call.
		// that is, execute all the draw calls that have been called since the last update.
		// On cleanup, we need to empty the draw calls queue by calling update, then delete the game objects and make dure no more draw() function calls are made
		system->update();

 		Cleanup();

		ending_game = false;
	}

	return true;
}


/**
* Update all the game objects in the game:
*   - The player
*   - The aliens (alien formation)
*   - Player rockets
*/
void Game::UpdateGameObjects(float delta_time)
{
	// update the game objects
	if (player != nullptr)
	{
		player->Update(delta_time);
	}

	if (aliens != nullptr)
	{
		aliens->Update(delta_time);
		aliens->UpdateRocketCollisions(player->GetPlayerRockets());

		// Update the falling bombs
		aliens->UpdateBombs(delta_time);

		if (aliens->AliensCount() == 0)
		{
			aliens->Create(5, 5);
			aliens->Reset();
		}
	}
}

void Game::UpdateUI(float delta_time)
{
	if (game_ui != nullptr)
	{
		game_ui->Update(delta_time);
	}
}