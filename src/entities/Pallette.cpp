#include "Pallette.h"
#include "../screen/GameplayScreen.h"
#include "../utilities/Constants.h"

#include <sl.h>

// TEXTURES

static int pallette;

void InitPallette()
{
	pallette = slLoadTexture("res/images/pallette/pallette.png");

	SetPalleteDefault();
}

void UpdatePallete()
{
	if (Gameplay::pall.x - Gameplay::pall.width / 2.0 < 0)
	{
		Gameplay::pall.x = Gameplay::pall.width / 2.0;
	}
	if (Gameplay::pall.x + Gameplay::pall.width / 2.0 > SCREEN_WIDTH)
	{
		Gameplay::pall.x = SCREEN_WIDTH - Gameplay::pall.width / 2;
	}
}

void DrawPallete()
{
	slSprite(pallette, Gameplay::pall.x, Gameplay::pall.y, Gameplay::pall.width, Gameplay::pall.height);
}

void SetPalleteDefault()
{
	Gameplay::pall.width = 100.0;
	Gameplay::pall.height = 50.0;
	SetPalleteDefaultPosition();
	Gameplay::pall.speed = 700.0;
	Gameplay::pall.lives = 3;
	Gameplay::pall.score = 0;
	Gameplay::pall.isWinner = false;
}

void SetPalleteDefaultPosition()
{
	Gameplay::pall.x = SCREEN_WIDTH / 2.0;
	Gameplay::pall.y = Gameplay::pall.height;
}