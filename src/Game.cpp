#include "Game.h"
#include "sl.h"
#include "utilities/Constants.h"
#include "screen/MainMenuScreen.h"
#include "screen/GameplayScreen.h"
#include "screen/HowToPlayScreen.h"
#include "screen//CreditsScreen.h"

namespace CatBounce
{
	Scenes currentScene = Scenes::MainMenu;
	bool isRunning = true;
	int font;

	void Game()
	{
		slWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROGRAM_NAME.c_str(), false);
		font = slLoadFont("res/fonts/Roboto.ttf");
		slSetFont(font, 24);
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
		MainMenu::Init();
		Gameplay::Init();
		HowToPlay::Init();
		Credits::Init();
	}

	void Input()
	{
		switch (currentScene)
		{
		case CatBounce::Scenes::MainMenu:

			MainMenu::Input();

			break;
		case CatBounce::Scenes::Gameplay:

			Gameplay::Input();

			break;
		case CatBounce::Scenes::HowToPlay:

			HowToPlay::Input();

			break;
		case CatBounce::Scenes::Credits:

			Credits::Input();

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	void Update()
	{
		switch (currentScene)
		{
		case CatBounce::Scenes::MainMenu:

			MainMenu::Update();

			break;
		case CatBounce::Scenes::Gameplay:

			Gameplay::Update();

			break;
		case CatBounce::Scenes::HowToPlay:

			HowToPlay::Update();

			break;
		case CatBounce::Scenes::Credits:

			Credits::Update();

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}

	void Draw()
	{
		switch (currentScene)
		{
		case CatBounce::Scenes::MainMenu:

			MainMenu::Draw();

			break;
		case CatBounce::Scenes::Gameplay:

			Gameplay::Draw();

			break;
		case CatBounce::Scenes::HowToPlay:

			HowToPlay::Draw();

			break;
		case CatBounce::Scenes::Credits:

			Credits::Draw();

			break;
		default:

			// THERE ARE NO OTHER SCENES

			break;
		}
	}
}