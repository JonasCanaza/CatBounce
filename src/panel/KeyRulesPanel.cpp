#include "KeyRulesPanel.h"
#include "../Game.h"
#include "../interface/Button.h"
#include "../utilities/Constants.h"
#include "../screen/MainMenuScreen.h"

#include <sl.h>

namespace KeyRulesPanel
{
	bool isActive;
	static int texts;

	static Button button;
	static const std::string buttonName = "BACK";

	void Init()
	{
		isActive = false;
		texts = slLoadTexture("res/images/ui/keysText.png");

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
			isActive = false;
		}
	}

	void Update()
	{
		UpdateButton(button);

		if (button.clicked)
		{
			slSoundPlay(buttonPressedSound);
			isActive = false;
		}

		MainMenu::UpdateBackground();
	}

	void Draw()
	{
		slSetForeColor(0.0, 0.0, 0.0, 0.85);
		slRectangleFill(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);

		slSprite(texts, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);

		DrawButton(button);
	}
}