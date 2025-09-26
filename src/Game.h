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

	extern int buttonPressed;

	extern int normalBallTexture;

	extern int normalFishTexture;
	extern int fireFishTexture;
	extern int speedFishTexture;
	extern int slownessFishTexture;

	extern int normalButtonTexture;
	extern int hoverButtonTexture;
	extern int pressedButtonTexture;

	void Game();
	void Init();
	void Input();
	void Update();
	void Draw();
}