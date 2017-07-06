#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

#include "resource.h"
#include "DiceInvaders.h"

/**
 * UserInterface
 * UI and menu item
 *
 **/

class UserInterface
{
public:

	enum MenuState { IN_GAME, LOBBY, GAME_OVER };

	UserInterface(class Game* the_game);
	~UserInterface();

	virtual bool Create(const char * spite_path);
	virtual void Destroy();

	virtual bool Update(float delta_time);

	void SetState(MenuState new_state) { state = new_state; }

	void SetAmmo(int ammo) { player_ammo = ammo; }
	void SetScore(int score) { player_score = score; }
	void SetHealth(int health) { player_health = health; }

	void UpdateInputs(float delta_time);
	
protected:

	MenuState state;
	class Game* game;

private:

	void PrintScore();
	void PrintStats();
	void PrintHealth();
	void PrintGameOver();

	static constexpr int kTextBufferSize = 64;
	char text_buffer[kTextBufferSize];
	int player_ammo;
	int player_score;
	int player_health;
};



#endif //__USERINTERFACE_H__
