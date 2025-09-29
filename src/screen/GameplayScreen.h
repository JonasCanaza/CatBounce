#pragma once
#include "../entities/Pallette.h"
#include "../entities/Ball.h"
#include "../entities/Fish.h"
#include "../entities/PowerItem.h"

namespace Gameplay
{
	extern bool isGameOver;
	extern int gameplayMusic;
	extern int gameplayMusicLoop;

	extern Pallette pall;
	extern Ball ball;
	extern Fish fish[MAX_ROW_FISH][MAX_COL_FISH];
	extern PowerItem powerItem[MAX_FISH_SPECIALS];

	void Init();
	void Input();
	void Update();
	void Draw();

	void ResetBall();
}