#include "Game.h"
#include "sl.h"
#include "utilities/Constants.h"
#include "scenes/GameplayScreen.h"

namespace CatBounce
{
	Scenes currentScene = Scenes::Gameplay;
	bool isRunning = true;

	void Game()
	{
		slWindow(WIDTH_SCREEN, HEIGHT_SCREEN, PROGRAM_NAME.c_str(), false);
		Init();

		while (!slShouldClose() && isRunning)
		{
			Input();
			Update();
			Draw();
		}

		slClose();
	}

	void Init()
	{
		Gameplay::Init();
	}

	void Input()
	{
		Gameplay::Input();
	}

	void Update()
	{
		Gameplay::Update();
	}

	void Draw()
	{
		// THERE ARE NO OTHER SCENES
		Gameplay::Draw();
	}
}