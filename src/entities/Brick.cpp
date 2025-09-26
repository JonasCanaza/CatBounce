#include "Brick.h"
#include "../utilities/Constants.h"
#include "../Game.h"

#include "sl.h"
#include <iostream>

static int specialFishCount = 0;

void InitBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS])
{
	specialFishCount = 0;
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
			bricks[row][col].width = brickWidth;
			bricks[row][col].height = brickHeight;
			bricks[row][col].x = offsetX + brickWidth / 2.0 + col * (brickWidth + spacing);
			bricks[row][col].y = SCREEN_HEIGHT - (marginY + brickHeight / 2.0 + row * (brickHeight + spacing));
			bricks[row][col].texture = 0;
			bricks[row][col].isActive = true;
			SetBrickType(bricks[row][col]);
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
				switch (bricks[row][col].type)
				{
				case BrickType::Normal:

					slSprite(CatBounce::normalFishTexture, bricks[row][col].x, bricks[row][col].y, bricks[row][col].width, bricks[row][col].height);

					break;
				case BrickType::Fire:

					slSprite(CatBounce::fireFishTexture, bricks[row][col].x, bricks[row][col].y, bricks[row][col].width, bricks[row][col].height);

					break;
				case BrickType::Speed:

					slSprite(CatBounce::speedFishTexture, bricks[row][col].x, bricks[row][col].y, bricks[row][col].width, bricks[row][col].height);

					break;
				case BrickType::Slowness:

					slSprite(CatBounce::slownessFishTexture, bricks[row][col].x, bricks[row][col].y, bricks[row][col].width, bricks[row][col].height);

					break;
				default:
					break;
				}
			}
		}
	}
}

void SetBrickType(Brick& bricks)
{
	bool confirmValue = false;
	int bricksSelection = 0;

	do
	{
		bricksSelection = rand() % MAX_BRICKS_TYPES + 1;

		if (specialFishCount >= MAX_BRICKS_SPECIALS)
		{
			if (bricksSelection == static_cast<int>(BrickType::Normal))
			{
				confirmValue = true;
			}
		}
		else
		{
			confirmValue = true;
		}
	} while (!confirmValue);

	if (bricksSelection != static_cast<int>(BrickType::Normal))
	{
		specialFishCount++;
		std::cout << specialFishCount << std::endl;
	}

	switch (static_cast<BrickType>(bricksSelection))
	{
	case BrickType::Normal:

		bricks.type = BrickType::Normal;
		std::cout << "Fish normal" << std::endl;

		break;
	case BrickType::Fire:

		bricks.type = BrickType::Fire;
		std::cout << "Fish fire" << std::endl;

		break;
	case BrickType::Speed:

		bricks.type = BrickType::Speed;
		std::cout << "Fish speed" << std::endl;

		break;
	case BrickType::Slowness:

		bricks.type = BrickType::Slowness;
		std::cout << "Fish slowness" << std::endl;

		break;
	default:
		break;
	}
}