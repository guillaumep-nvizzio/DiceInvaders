#ifndef __GAME_H__
#define __GAME_H__

#include "resource.h"
#include "DiceInvaders.h"

// definition of all the data path for sprites
#define PLAYER_SPRITE_PATH	"data/player.bmp"
#define ALIEN1_SPRITE_PATH	"data/enemy1.bmp"
#define ALIEN2_SPRITE_PATH	"data/enemy2.bmp"
#define ROCKET_SPRITE_PATH	"data/rocket.bmp"
#define BOMB_SPRITE_PATH	"data/bomb.bmp"

// gameplay constants
#define SPRITE_SIZE			32
#define PLAYER_SPEED		160.0f

/**
 * Game
 * Class that handles the high-level functionalities of the game as well as managing the different game objects
 *
 **/

class Game
{
public:

	Game();
	~Game();

	bool Initialize(IDiceInvaders* sys, int width, int height);
	void Deinitialize();

	bool Update(float delta_time);

	IDiceInvaders* GetSystem() {  return system; }
	int GetScreenHeight() { return screen_height; };
	int GetScreenWidth() { return screen_width; };

	class UserInterface *GetUI() { return game_ui; };
	class Player *GetPlayer() { return player; };

	void StartGame();
	void EndGame();
	
private:

	class Player *player;
	class AlienFormation *aliens;

	void CreatePlayer();
	void CreateAliensFormation();

	void Cleanup();

	void UpdateGameObjects(float delta_time);
	void UpdateUI(float delta_time);

	bool is_initialized;
	IDiceInvaders* system;

	int screen_height;
	int screen_width;

	bool playing;
	bool ending_game;

	class UserInterface *game_ui;
};


#endif //__GAME_H__