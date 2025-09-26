#pragma once

const int MIN_BRICKS = 1;
const int MAX_COL_BRICKS = 7;
const int MAX_ROW_BRICKS = 5;
const int MAX_BRICKS_SPECIALS = 5;
const int MAX_BRICKS_TYPES = 4;

enum class BrickType
{
	Normal = 1,
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
void SetBrickType(Brick& bricks);