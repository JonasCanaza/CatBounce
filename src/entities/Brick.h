#pragma once

const int MAX_COL_BRICKS = 7;
const int MAX_ROW_BRICKS = 5;

struct Brick
{
	double width;
	double height;
	double x;
	double y;
	int texture;
	bool isActive;
};

void InitBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS]);
void DrawBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS]);