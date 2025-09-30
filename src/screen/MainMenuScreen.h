#pragma once

namespace MainMenu
{
	extern int mainMenuMusic;
	extern int mainMenuMusicLoop;

	struct Background
	{
		double width;
		double height;
		double x;
		double y;
		double speedX;
		double speedY;
	};

	void Init();
	void Input();
	void Update();
	void Draw();
	void UpdateBackground();
	void DrawBackground();
}