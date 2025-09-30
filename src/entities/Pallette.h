#pragma once

struct Pallette
{
	double width;
	double height;
	double x;
	double y;
	double speed;
	int lives;
	int score;
	bool isWinner;
};

void InitPallette();
void UpdatePallete(double deltaTime);
void DrawPallete();
void SetPalleteDefault();
void SetPalleteDefaultPosition();