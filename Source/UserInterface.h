#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

#include "resource.h"
#include "DiceInvaders.h"

/**
 * UserInterface
 * UI and menu items
 * Straighforward class to print out the player score, health and stats.
 * It will listen to player inputs if the game is in GAME_OVER mode
 *
 **/

class UserInterface
{
public:

	enum MenuState { IN_GAME, GAME_OVER };

	UserInterface(class Game* the_game);
	~UserInterface();

	virtual bool Create(const char * spite_path);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	void UpdateInputs(float delta_time);

	void SetState(MenuState new_state) { state = new_state; }

	void SetAmmo(int ammo) { player_ammo = ammo; }
	void SetScore(int score) { player_score = score; }
	void SetHealth(int health) { player_health = health; }

private:

	void PrintScore();
	void PrintStats();
	void PrintHealth();
	void PrintGameOver();

	// character array used for printing text on screen, one buffer to avoid multiple allocation on each update
	static constexpr int kTextBufferSize = 64;
	char text_buffer[kTextBufferSize];

	int player_ammo;
	int player_score;
	int player_health;

	MenuState state;
	class Game* game;
};



#endif //__USERINTERFACE_H__
