#include "PausePanel.h"
#include "../interface/Button.h"
#include "../input/InputSystem.h"
#include "../Game.h"
#include "../utilities/Constants.h"
#include "../screen/MainMenuScreen.h"
#include "../screen/GameplayScreen.h"

#include <sl.h>
#include <iostream>

namespace PausePanel
{
	bool isActive;

	static const int MAX_BUTTONS = 3;
	static Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "RESUME", "RESTART", "EXIT" };
	static std::string titleText = "PAUSED";

	void Init()
	{
		isActive = false;

		double btnWidth = 250.0;
		double btnHeight = 75.0;
		double marginBetween = 10.0;

		double textHeight = slGetTextHeight(titleText.c_str());
		double spaceTextToButtons = 40.0;

		double totalHeight = textHeight + spaceTextToButtons + (btnHeight * MAX_BUTTONS) + (marginBetween * (MAX_BUTTONS - 1));
		double startY = (SCREEN_HEIGHT / 2.0) + (totalHeight / 2.0);

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			double y = startY - textHeight - spaceTextToButtons - (btnHeight / 2.0) - i * (btnHeight + marginBetween);
			buttons[i] = CreateButton((SCREEN_WIDTH / 2.0) - (btnWidth / 2.0), y, btnWidth, btnHeight, buttonNames[i]);
		}
	}

	void Update()
	{
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			UpdateButton(buttons[i]);

			if (buttons[i].clicked)
			{
				if (buttonNames[i] == "RESUME")
				{
					slSoundPlay(CatBounce::buttonPressedSound);
					slSoundResumeAll();

					isActive = false;
				}
				else if (buttonNames[i] == "RESTART")
				{
					slSoundPlay(CatBounce::buttonPressedSound);
					slSoundStop(Gameplay::gameplayMusicLoop);
					Gameplay::gameplayMusicLoop = slSoundLoop(Gameplay::gameplayMusic);

					Gameplay::ResetBall();
					SetupFishTypes(Gameplay::fish);
					Gameplay::pall.lives = 3;
					Gameplay::pall.score = 0;

					isActive = false;
				}
				else if (buttonNames[i] == "EXIT")
				{
					slSoundPlay(CatBounce::buttonPressedSound);
					slSoundStop(Gameplay::gameplayMusicLoop);
					MainMenu::mainMenuMusicLoop = slSoundLoop(MainMenu::mainMenuMusic);

					isActive = false;
					CatBounce::currentScene = CatBounce::Scenes::MainMenu;
				}
			}
		}
	}

	void Draw()
	{
		slSetForeColor(0.0, 0.0, 0.0, 0.85);
		slRectangleFill(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);

		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSetFont(CatBounce::specialFont, 70);

		double textWidth = slGetTextWidth(titleText.c_str());
		double textHeight = slGetTextHeight(titleText.c_str());
		double btnHeight = 75.0;
		double marginBetween = 10.0;
		double spaceTextToButtons = 40.0;

		double totalHeight = textHeight + spaceTextToButtons + (btnHeight * MAX_BUTTONS) + (marginBetween * (MAX_BUTTONS - 1));
		double startY = (SCREEN_HEIGHT / 2.0) + (totalHeight / 2.0);
		double textY = startY - (textHeight / 2.0);

		slText(SCREEN_WIDTH / 2.0 - textWidth / 2.0, textY, titleText.c_str());

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			DrawButton(buttons[i]);
		}
	}
}