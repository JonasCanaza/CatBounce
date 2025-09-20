#include "Game.h"
#include "sl.h"
#include "utilities/Constants.h"
#include "screen/MainMenuScreen.h"
#include "screen/GameplayScreen.h"
#include "screen/HowToPlayScreen.h"
#include "screen//CreditsScreen.h"

namespace CatBounce
{
	Scenes currentScene = Scenes::Gameplay;
	bool isRunning = true;

	int normalFont;
	int specialFont;

	int normalBallTexture;

	int normalFishTexture;

	int normalButtonTexture;
	int hoverButtonTexture;
	int pressedButtonTexture;

	void Game()
	{
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

	void Init()
	{
		normalButtonTexture = slLoadTexture("res/images/normalButton.png");
		hoverButtonTexture = slLoadTexture("res/images/hoverButton.png");
		pressedButtonTexture = slLoadTexture("res/images/pressedButton.png");
		normalBallTexture = slLoadTexture("res/images/normalBall.png");
		normalFishTexture = slLoadTexture("res/images/normalFish.png");

		normalFont = slLoadFont("res/fonts/Roboto.ttf");
		specialFont = slLoadFont("res/fonts/pusab/PUSAB___.ttf");

		slSetFont(normalFont, 24);

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