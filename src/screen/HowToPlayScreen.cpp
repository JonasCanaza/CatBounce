#include "HowToPlayScreen.h"
#include "../utilities/Constants.h"
#include "../Game.h"

#include "sl.h"

namespace HowToPlay
{
	void Init()
	{

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

	}

	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);

		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slText(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, "How to play screen");

		slRender();
	}
}