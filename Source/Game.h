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

// engine loop: delay to obtain 60 FPS maximum
#define ENGINE_LOOP_DELAY	1000.0f / 60.0f

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

	// put the game in the "Play" state: create the player and the alien formation
	void StartGame();

	// this function will set a flag to cleanup the game objects and end the game on the next frame
	void EndGame();
	
private:

	// create the player game object
	void CreatePlayer();

	// create the aliens formation
	void CreateAliensFormation();

	// delete the player and alien formation game objects
	void Cleanup();

	void UpdateGameObjects(float delta_time);
	void UpdateUI(float delta_time);

	bool is_initialized;
	
	// screen size as defined in Ganme class initialization
	int screen_height;
	int screen_width;

	// game state flags
	bool playing;			// are we playing right now
	bool ending_game;		// if true; the game will be terminated on the next frame

	// the size of the alien formation
	static constexpr int kFormationSizeColumns = 5;
	static constexpr int kFormationSizeRows = 5;

	// pointers to player, aliens and user interface objects:
	//  - created (allocated) in this class
	class Player *player;
	class AlienFormation *aliens;
	class UserInterface *game_ui;

	// A pointer to the DiceInvader system library
	IDiceInvaders* system;
};


#endif //__GAME_H__