#include "MainMenuScreen.h"
#include "../interface/Button.h"
#include "../Game.h"
#include "../utilities/Constants.h"
#include <iostream>
#include "sl.h"

namespace MainMenu
{
	static const int MAX_BUTTONS = 4;
	static Button buttons[MAX_BUTTONS];
	static std::string buttonNames[MAX_BUTTONS] = { "Play", "How to play", "Credits", "Exit" };

	void Init()
	{
		double btnWidth = 250.0;
		double btnHeight = 75.0;
		double marginBetween = 10.0;
		double marginBottom = 300.0;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			double y = SCREEN_HEIGHT - marginBottom - (btnHeight + marginBetween) * (i + 1);

			buttons[i] = CreateButton((SCREEN_WIDTH / 2.0) - (btnWidth / 2.0), y, btnWidth, btnHeight, buttonNames[MAX_BUTTONS - 1 - i]);
		}
	}

	void Input()
	{
		if (slGetKey(SL_KEY_ESCAPE))
		{
			CatBounce::isRunning = false;
		}
	}

	void Update()
	{
		//std::cout << "X: " << slGetMouseX() << "          Y: " << slGetMouseY() << std::endl;

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			UpdateButton(buttons[i]);
		}

		if (buttons[0].clicked)
		{
			CatBounce::currentScene = CatBounce::Scenes::Gameplay;
		}
		if (buttons[1].clicked)
		{
			CatBounce::currentScene = CatBounce::Scenes::HowToPlay;
		}
		if (buttons[2].clicked)
		{
			CatBounce::currentScene = CatBounce::Scenes::Credits;
		}
		if (buttons[3].clicked)
		{
			CatBounce::isRunning = false;
		}
	}
	
	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);
		
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			DrawButton(buttons[i]);
		}

		slRender();
	}
}