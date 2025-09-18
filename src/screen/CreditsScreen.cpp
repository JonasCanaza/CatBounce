#include "CreditsScreen.h"
#include "sl.h"
#include "../utilities/Constants.h"
#include "../Game.h"

namespace Credits
{
	void Init()
	{

	}

	void Input()
	{
		if (slGetKey(SL_KEY_ESCAPE))
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
		slText(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, "Credits screen");

		slRender();
	}
}