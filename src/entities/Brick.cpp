#include "Brick.h"
#include "sl.h"
#include "../utilities/Constants.h"
#include "../Game.h"

void InitBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS])
{
	double brickWidth = 90.0;
	double brickHeight = 50.0;
	double spacing = 5.0;
	double marginY = MARGIN_Y_BRICKS + 50;
	double totalWidth = MAX_COL_BRICKS * brickWidth + (MAX_COL_BRICKS - 1) * spacing;
	double offsetX = (SCREEN_WIDTH - totalWidth) / 2.0;

	for (int row = 0; row < MAX_ROW_BRICKS; row++)
	{
		for (int col = 0; col < MAX_COL_BRICKS; col++)
		{
			if (row == 4 && col == 3)
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
}

void DrawBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS])
{
	for (int row = 0; row < MAX_ROW_BRICKS; row++)
	{
		for (int col = 0; col < MAX_COL_BRICKS; col++)
		{
			if (bricks[row][col].isActive)
			{
				slSprite(CatBounce::normalFishTexture, bricks[row][col].x, bricks[row][col].y, bricks[row][col].width, bricks[row][col].height);

			}
		}
	}
}