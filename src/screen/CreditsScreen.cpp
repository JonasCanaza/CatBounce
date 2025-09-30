#include "CreditsScreen.h"
#include "../Game.h"
#include "../utilities/Constants.h"
#include "../interface/Button.h"

#include "sl.h"
#include <iostream>

namespace Credits
{
	static const std::string title = "Credits";
	static Button button;
	static const std::string buttonName = "Back";

	static void DrawCenteredText(std::string text, double y);

	void Init()
	{
		double btnWidth = 250.0;
		double btnHeight = 75.0;
		double marginButtom = 100.0;

		button = CreateButton(SCREEN_WIDTH / 2.0 - btnWidth / 2.0, marginButtom, btnWidth, btnHeight, buttonName);
	}

	void Input()
	{
		UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

		if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown)
		{
			CatBounce::currentScene = CatBounce::Scenes::MainMenu;
		}
	}

	void Update()
	{
		UpdateButton(button);

		if (button.clicked)
		{
			slSoundPlay(buttonPressedSound);
			CatBounce::currentScene = CatBounce::Scenes::MainMenu;
		}
	}

	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);

		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSetFont(CatBounce::specialFont, 70);

		double titleMarginTop = 60.0;
		double titlePosX = SCREEN_WIDTH / 2.0 - slGetTextWidth(title.c_str()) / 2.0;
		double titlePosY = SCREEN_HEIGHT - titleMarginTop;
		slText(titlePosX, titlePosY, title.c_str());

		double titleHeight = slGetTextHeight(title.c_str());

		slSetFont(CatBounce::specialFont, 35);
		double lineSpacing = 50.0;

		std::string lines[] = {
			"Developed by:", "CaNaZa", "",
			"Music by:", "Cafofo", "WoW Sound", "",
			"Sound effects by:", "Robtop Games", "",
			"Special dedications:", "Sergio Baretto", "Stefano Cvitanich"
		};

		int totalLines = sizeof(lines) / sizeof(lines[0]);
		double blockHeight = totalLines * lineSpacing;

		double topLimitY = titlePosY - titleHeight - 20.0;
		double bottomLimitY = button.y + button.height + 40.0;

		double availableHeight = topLimitY - bottomLimitY;

		double spacing = lineSpacing;

		if (blockHeight > availableHeight)
		{
			spacing = availableHeight / totalLines;
		}

		double startY = topLimitY - spacing;

		for (int i = 0; i < totalLines; i++)
		{
			DrawCenteredText(lines[i], startY - spacing * i);
		}

		DrawButton(button);

		slRender();
	}

	static void DrawCenteredText(std::string text, double y)
	{
		double posX = SCREEN_WIDTH / 2.0 - slGetTextWidth(text.c_str()) / 2.0;
		slText(posX, y, text.c_str());
	}
}