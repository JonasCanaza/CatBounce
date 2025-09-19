#include "Brick.h"
#include "sl.h"
#include "../utilities/Constants.h"

void InitBricks(Brick bricks[MAX_BRICKS][MAX_BRICKS])
{
	double brickWidth = 90.0;
	double brickHeight = 50.0;
	double spacing = 0.0;
	double marginY = 50.0;
	double totalWidth = MAX_BRICKS * brickWidth + (MAX_BRICKS - 1) * spacing;
	double offsetX = (SCREEN_WIDTH - totalWidth) / 2.0;

	for (int row = 0; row < MAX_BRICKS; row++)
	{
		for (int col = 0; col < MAX_BRICKS; col++)
		{
			bricks[row][col].width = brickWidth;
			bricks[row][col].height = brickHeight;
			bricks[row][col].x = offsetX + brickWidth / 2.0 + col * (brickWidth + spacing);
			bricks[row][col].y = SCREEN_HEIGHT - (marginY + brickHeight / 2.0 + row * (brickHeight + spacing));
			bricks[row][col].texture = 0;
			bricks[row][col].isActive = true;
		}
	}
}

void DrawBricks(Brick bricks[MAX_BRICKS][MAX_BRICKS])
{
	for (int row = 0; row < MAX_BRICKS; row++)
	{
		for (int col = 0; col < MAX_BRICKS; col++)
		{
			if (bricks[row][col].isActive)
			{
				slRectangleFill(bricks[row][col].x, bricks[row][col].y, bricks[row][col].width, bricks[row][col].height);
			}
		}
	}
}