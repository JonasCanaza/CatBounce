#include "Game.h"
#include "screen/MainMenuScreen.h"
#include "screen/GameplayScreen.h"
#include "screen/HowToPlayScreen.h"
#include "screen//CreditsScreen.h"
#include "utilities/Constants.h"
#include "interface/Button.h"

#include "sl.h"
#include <ctime>

namespace CatBounce
{
	Scenes currentScene = Scenes::MainMenu;
	bool isRunning = true;

	KeyTracker inputSystem;

	int normalFont;
	int specialFont;

	static void Init();
	static void Input();
	static void Update();
	static void Draw();

	void Game()
	{
		srand(time(0));
		slWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROGRAM_NAME.c_str(), false);
		Init();

		while (!slShouldClose() && isRunning)
		{
			Input();
			Update();
			Draw();
		}

		slClose();
	}

	static void Init()
	{
		InitKey(inputSystem);
		InitButton();

		// TEXT

		normalFont = slLoadFont("res/fonts/AROneSans/AROneSans-Regular.ttf");
		specialFont = slLoadFont("res/fonts/pusab/PUSAB___.ttf");

		slSetFont(normalFont, 24);

		MainMenu::Init();
		Gameplay::Init();
		HowToPlay::Init();
		Credits::Init();
	}

	static void Input()
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

	static void Update()
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

	static void Draw()
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