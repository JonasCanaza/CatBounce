#pragma once

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
	extern int font;

	extern int normalBallTexture;

	extern int normalFishTexture;

	extern int normalButtonTexture;
	extern int hoverButtonTexture;
	extern int pressedButtonTexture;

	void Game();
	void Init();
	void Input();
	void Update();
	void Draw();
}