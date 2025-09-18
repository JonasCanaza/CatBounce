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

	void Game();
	void Init();
	void Input();
	void Update();
	void Draw();
}