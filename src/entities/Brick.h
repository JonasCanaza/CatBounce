#pragma once

const int MAX_COL_BRICKS = 7;
const int MAX_ROW_BRICKS = 5;

enum class BrickType
{
	Normal,
	Fire,
	Speed,
	Slowness
};

struct Brick
{
	double width;
	double height;
	double x;
	double y;
	int texture;
	bool isActive;
	BrickType type;
};

void InitBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS]);
void DrawBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS]);