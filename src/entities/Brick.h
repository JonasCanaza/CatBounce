#pragma once

const int MAX_BRICKS = 5;

struct Brick
{
	double width;
	double height;
	double x;
	double y;
	int texture;
	bool isActive;
};

void InitBricks(Brick bricks[MAX_BRICKS][MAX_BRICKS]);
void DrawBricks(Brick bricks[MAX_BRICKS][MAX_BRICKS]);