// DiceInvaders.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <cassert>
#include <cstdio>

#include "Game.h"
#include "Dice.h"
#include "DiceInvaders.h"
#include "DiceLog.h"

class DiceInvadersLib
{
public:
	explicit DiceInvadersLib(const char* libraryPath)
	{
		// convert to wide char
		wchar_t* wLibraryPathStr = new wchar_t[512];
		MultiByteToWideChar(CP_ACP, 0, libraryPath, -1, wLibraryPathStr, 4096);

		m_lib = LoadLibrary(wLibraryPathStr);

		if (m_lib == nullptr)
		{
			DWORD Error = GetLastError();
			char buffer[512];
			snprintf(buffer, 512, "LoadLibrary Error: %d", Error);
			DICE_LOG_ERROR(buffer);
		}

		assert(m_lib);

		DiceInvadersFactoryType* factory = (DiceInvadersFactoryType*)GetProcAddress(
			m_lib, "DiceInvadersFactory");
		m_interface = factory();
		assert(m_interface);

		delete wLibraryPathStr;
	}

	~DiceInvadersLib()
	{
		FreeLibrary(m_lib);
	}

	IDiceInvaders* get() const
	{
		return m_interface;
	}

private:
	DiceInvadersLib(const DiceInvadersLib&);
	DiceInvadersLib& operator=(const DiceInvadersLib&);

private:
	IDiceInvaders* m_interface;
	HMODULE m_lib;
};

 
int APIENTRY WinMain( HINSTANCE instance, HINSTANCE previousInstance,LPSTR commandLine, int commandShow )
{
	//Log::ReportingLevel() = logVERBOSE;
	Log::ReportingLevel() = logINFO;

	DICE_LOG_INFO("Loading DiceInvaders.dll");

	DiceInvadersLib lib("DiceInvaders.dll");
	IDiceInvaders* system = lib.get();

	Game MyGame;

	bool init = MyGame.Initialize(system, 640, 480);
	if (init == false)
	{
		return -1;
	}

	bool running = true;
	float lastTime = system->getElapsedTime();
	float delta_time = system->getElapsedTime() - lastTime;

	MyGame.StartGame();

	while (running == true)
	{
		delta_time = system->getElapsedTime() - lastTime;

		running = MyGame.Update(delta_time);	

		lastTime = system->getElapsedTime();

		// 60 FPS ?
		Sleep(ENGINE_LOOP_DELAY);
	}

	return 0;
}



