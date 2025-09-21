#pragma once

namespace MainMenu
{
	struct Background
	{
		double width;
		double height;
		double x;
		double y;
		double speedX;
		double speedY;
	};

	extern int mainMenuMusic;
	extern int mainMenuMusicLoop;

	void Init();
	void Input();
	void Update();
	void Draw();
}