#include "HowToPlayScreen.h"
#include "../Game.h"
#include "../utilities/Constants.h"
#include "../interface/Button.h"
#include "../panel/GeneralRulesPanel.h"
#include "../panel/SpellRulesPanel.h"
#include "../panel/KeyRulesPanel.h"
#include "MainMenuScreen.h"

#include "sl.h"
#include <iostream>

namespace HowToPlay
{
	static const std::string title = "RULES";

	static const int MAX_MAIN_BUTTONS = 3;
	static Button mainButtons[MAX_MAIN_BUTTONS];
	static std::string mainButtonNames[MAX_MAIN_BUTTONS] = { "GENERAL", "SPELLS", "KEYS" };

	static Button backButton;
	static const std::string backButtonName = "BACK";

	void Init()
	{
		double btnWidth = 250.0;
		double btnHeight = 75.0;
		double marginBetween = 10.0;
		double marginTop = 180.0;

		for (int i = 0; i < MAX_MAIN_BUTTONS; i++)
		{
			double y = SCREEN_HEIGHT - marginTop - (btnHeight + marginBetween) * (i + 1);

			mainButtons[i] = CreateButton((SCREEN_WIDTH / 2.0) - (btnWidth / 2.0), y, btnWidth, btnHeight, mainButtonNames[i]);
		}

		double marginBottom = 100.0;

		backButton = CreateButton(SCREEN_WIDTH / 2.0 - btnWidth / 2.0, marginBottom, btnWidth, btnHeight, backButtonName);

		GeneralRulesPanel::Init();
		SpellRulesPanel::Init();
		KeyRulesPanel::Init();
	}

	void Input()
	{
		if (!GeneralRulesPanel::isActive && !SpellRulesPanel::isActive && !KeyRulesPanel::isActive)
		{
			UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

			if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown)
			{
				CatBounce::currentScene = CatBounce::Scenes::MainMenu;
			}
		}
		else
		{
			if (GeneralRulesPanel::isActive)
			{
				GeneralRulesPanel::Input();
			}

			if (SpellRulesPanel::isActive)
			{
				SpellRulesPanel::Input();
			}

			if (KeyRulesPanel::isActive)
			{
				KeyRulesPanel::Input();
			}
		}
	}

	void Update()
	{
		if (!GeneralRulesPanel::isActive && !SpellRulesPanel::isActive && !KeyRulesPanel::isActive)
		{
			for (int i = 0; i < MAX_MAIN_BUTTONS; i++)
			{
				UpdateButton(mainButtons[i]);
			}

			if (mainButtons[0].clicked)
			{
				slSoundPlay(buttonPressedSound);
				GeneralRulesPanel::isActive = true;
			}

			if (mainButtons[1].clicked)
			{
				slSoundPlay(buttonPressedSound);
				SpellRulesPanel::isActive = true;
			}

			if (mainButtons[2].clicked)
			{
				slSoundPlay(buttonPressedSound);
				KeyRulesPanel::isActive = true;
			}

			UpdateButton(backButton);

			if (backButton.clicked)
			{
				slSoundPlay(buttonPressedSound);
				CatBounce::currentScene = CatBounce::Scenes::MainMenu;
			}
		}
		else
		{
			if (GeneralRulesPanel::isActive)
			{
				GeneralRulesPanel::Update();
			}

			if (SpellRulesPanel::isActive)
			{
				SpellRulesPanel::Update();
			}

			if (KeyRulesPanel::isActive)
			{
				KeyRulesPanel::Update();
			}
		}

		MainMenu::UpdateBackground();
	}

	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);

		MainMenu::DrawBackground();
		slSetForeColor(0.0, 0.0, 0.0, 0.4);
		slRectangleFill(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);

		slSetFont(CatBounce::specialFont, 70);

		double titleMarginTop = 70.0;
		double titlePosX = SCREEN_WIDTH / 2.0 - slGetTextWidth(title.c_str()) / 2.0;
		double titlePosY = SCREEN_HEIGHT - titleMarginTop;
		slText(titlePosX, titlePosY, title.c_str());

		for (int i = 0; i < MAX_MAIN_BUTTONS; i++)
		{
			DrawButton(mainButtons[i]);
		}

		DrawButton(backButton);

		if (GeneralRulesPanel::isActive)
		{
			GeneralRulesPanel::Draw();
		}

		if (SpellRulesPanel::isActive)
		{
			SpellRulesPanel::Draw();
		}

		if (KeyRulesPanel::isActive)
		{
			KeyRulesPanel::Draw();
		}

		slRender();
	}
}