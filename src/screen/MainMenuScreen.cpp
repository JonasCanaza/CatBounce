#include "MainMenuScreen.h"
#include "GameplayScreen.h"
#include "../interface/Button.h"
#include "../Game.h"
#include "../utilities/Constants.h"
#include "../panel/ExitPanel.h"
#include "../entities/Fish.h"
#include "../entities/PowerItem.h"

#include <iostream>
#include "sl.h"

namespace MainMenu
{
	static int logo;
	static int mainMenuBackground;
	int mainMenuMusic;
	int mainMenuMusicLoop;

	static Background bgOne;
	static const int MAX_BUTTONS = 4;
	static Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "PLAY", "RULES", "CREDITS", "EXIT" };

	void Init()
	{
		ExitPanel::Init();

		bgOne.width = SCREEN_WIDTH;
		bgOne.height = SCREEN_HEIGHT;
		bgOne.x = SCREEN_WIDTH / 2.0;
		bgOne.y = SCREEN_HEIGHT / 2.0;
		bgOne.speedX = 10.0;
		bgOne.speedY = -10.0;

		logo = slLoadTexture("res/images/ui/logo.png");
		mainMenuBackground = slLoadTexture("res/images/background/mainMenu.png");
		mainMenuMusic = slLoadWAV("res/music/mainMenuMusic.wav");

		mainMenuMusicLoop = slSoundLoop(mainMenuMusic);

		double btnWidth = 250.0;
		double btnHeight = 75.0;
		double marginBetween = 10.0;
		double marginBottom = 300.0;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			double y = SCREEN_HEIGHT - marginBottom - (btnHeight + marginBetween) * (i + 1);

			buttons[i] = CreateButton((SCREEN_WIDTH / 2.0) - (btnWidth / 2.0), y, btnWidth, btnHeight, buttonNames[i]);
		}
	}

	void Input()
	{
		UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

		if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown)
		{
			ExitPanel::isActive = !ExitPanel::isActive;
		}
	}

	void Update()
	{
		if (!ExitPanel::isActive)
		{
			for (int i = 0; i < MAX_BUTTONS; i++)
			{
				UpdateButton(buttons[i]);
			}

			if (buttons[0].clicked)
			{
				slSoundPlay(CatBounce::buttonPressed);
				slSoundStop(mainMenuMusicLoop);
				Gameplay::gameplayMusicLoop = slSoundLoop(Gameplay::gameplayMusic);
				SetupFishTypes(Gameplay::fish);
				CatBounce::currentScene = CatBounce::Scenes::Gameplay;
			}

			if (buttons[1].clicked)
			{
				slSoundPlay(CatBounce::buttonPressed);
				CatBounce::currentScene = CatBounce::Scenes::HowToPlay;
			}

			if (buttons[2].clicked)
			{
				slSoundPlay(CatBounce::buttonPressed);
				CatBounce::currentScene = CatBounce::Scenes::Credits;
			}

			if (buttons[3].clicked)
			{
				slSoundPlay(CatBounce::buttonPressed);
				ExitPanel::isActive = !ExitPanel::isActive;
			}
		}
		else
		{
			ExitPanel::Update();
		}

		bgOne.x += bgOne.speedX * slGetDeltaTime();
		bgOne.y += bgOne.speedY * slGetDeltaTime();

		if (bgOne.x >= SCREEN_WIDTH / 2.0 + bgOne.width)
		{
			bgOne.x -= bgOne.width;
		}

		if (bgOne.x <= SCREEN_WIDTH / 2.0 - bgOne.width)
		{
			bgOne.x += bgOne.width;
		}

		if (bgOne.y >= SCREEN_HEIGHT / 2.0 + bgOne.height)
		{
			bgOne.y -= bgOne.height;
		}

		if (bgOne.y <= SCREEN_HEIGHT / 2.0 - bgOne.height)
		{
			bgOne.y += bgOne.height;
		}
	}
	
	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);
		slSprite(mainMenuBackground, bgOne.x, bgOne.y, bgOne.width, bgOne.height);

		slSprite(mainMenuBackground, bgOne.x - bgOne.width, bgOne.y, bgOne.width, bgOne.height);
		slSprite(mainMenuBackground, bgOne.x + bgOne.width, bgOne.y, bgOne.width, bgOne.height);
		slSprite(mainMenuBackground, bgOne.x, bgOne.y - bgOne.height, bgOne.width, bgOne.height);
		slSprite(mainMenuBackground, bgOne.x, bgOne.y + bgOne.height, bgOne.width, bgOne.height);

		slSprite(mainMenuBackground, bgOne.x - bgOne.width, bgOne.y - bgOne.height, bgOne.width, bgOne.height);
		slSprite(mainMenuBackground, bgOne.x + bgOne.width, bgOne.y - bgOne.height, bgOne.width, bgOne.height);
		slSprite(mainMenuBackground, bgOne.x - bgOne.width, bgOne.y + bgOne.height, bgOne.width, bgOne.height);
		slSprite(mainMenuBackground, bgOne.x + bgOne.width, bgOne.y + bgOne.height, bgOne.width, bgOne.height);

		slSprite(logo, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0 + 200, 450, 350);

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			DrawButton(buttons[i]);
		}

		if (ExitPanel::isActive)
		{
			ExitPanel::Draw();
		}

		slRender();
	}
}