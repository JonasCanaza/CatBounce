#pragma once
#include "../entities/Pallette.h"
#include "../entities/Ball.h"
#include "../entities/Brick.h"

namespace Gameplay
{
	extern bool isGameOver;
	extern int gameplayMusic;
	extern int gameplayMusicLoop;

	extern Pallette pall;
	extern Ball ball;
	extern Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS];

	void Init();
	void Input();
	void Update();
	void Draw();
}