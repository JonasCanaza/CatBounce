#pragma once
#include "input/InputSystem.h"

namespace CatBounce
{
	enum class Scenes
	{
		MainMenu,
		Gameplay,
		HowToPlay,
		Credits
	};

	extern Scenes currentScene;
	extern bool isRunning;

	extern KeyTracker inputSystem;

	extern int normalFont;
	extern int specialFont;

	void Game();
}