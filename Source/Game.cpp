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

/**
* Start the game by creating the game objects and updating the internal flags
*/
void Game::StartGame()
{
	// Create the player, and aliens...
	CreatePlayer();
	CreateAliensFormation();
	playing = true;
	ending_game = false;
}

/**
* Set the ending_game flag. On the next frame, the game will restart: cleanup game objects and re-create alien formation
*/
void Game::EndGame()
{
	ending_game = true;
}

/**
* Delete the player and the aliens formation game objects
*/
void Game::Cleanup()
{
	delete player;
	player = nullptr;

	delete aliens;
	aliens = nullptr;

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

	aliens->Create(kFormationSizeRows, kFormationSizeColumns);
}

/**
* Update the game:
*  - system update
*  - user interface
*  - when in playing mode: updated the game objects
*  - if the ending_game flag is set, game is over so cleanup the game objects and set the UI in game over mode
*/
bool Game::Update(float delta_time)
{
	if (system->update() == false)
	{
		DICE_LOG_ERROR("Error on System Update");
		return false;
	}

	// update the user interface
	UpdateUI(delta_time);

	// update game objects if the game is ni play modes
	if (playing == true)
	{
		UpdateGameObjects(delta_time);
	}
	
	if (ending_game == true)
	{
		// system->update() will clears the screen and draws all sprites and texts which have been drawn since the last update call.
		// that is, execute all the draw calls that have been called since the last update.
		// On cleanup, we need to empty the draw calls queue by calling update, then delete the game objects and make dure no more draw() function calls are made
		// If we call system->update() with draw calls in the queue and the objects are deleted, it will crash.
		system->update();

 		Cleanup();

		// set the current game state to GAME_OVER, see UserInterface.cpp for details on that state behavior
		game_ui->SetState(UserInterface::MenuState::GAME_OVER);

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

		// reset the game: if no more aliens are present, we re-create the alien formation and reset the internal values for speed and movement
		if (aliens->AliensCount() == 0)
		{
			aliens->Create(5, 5);
			aliens->Reset();
		}
	}
}

/**
* Update the user interface
*/
void Game::UpdateUI(float delta_time)
{
	if (game_ui != nullptr)
	{
		game_ui->Update(delta_time);
	}
}