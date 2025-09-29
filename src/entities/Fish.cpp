#include "Fish.h"
#include "../utilities/Constants.h"
#include "../Game.h"

#include "sl.h"
#include <iostream>

static int normalFishTexture;
static int fireFishTexture;
static int speedFishTexture;
static int slownessFishTexture;
static int fishStoneIntactTexture;
static int fishStoneCrackedTexture;
static int fishStoneBrokenTexture;
static int fishStoneFracturedTexture;

static const int TOTAL_FISH_SPECIALS = 3;
static const int MAX_FISH_SPECIALS = 5;
static const int MAX_FISH_ROCK = 10;

static void SetFishPositions(Fish fish[MAX_ROW_FISH][MAX_COL_FISH]);

void InitFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	normalFishTexture = slLoadTexture("res/images/fish/normalFish.png");
	fireFishTexture = slLoadTexture("res/images/fish/fireFish.png");
	speedFishTexture = slLoadTexture("res/images/fish/speedFish.png");
	slownessFishTexture = slLoadTexture("res/images/fish/slownessFish.png");
	fishStoneIntactTexture = slLoadTexture("res/images/fish/fishStoneIntact.png");
	fishStoneCrackedTexture = slLoadTexture("res/images/fish/fishStoneCracked.png");
	fishStoneBrokenTexture = slLoadTexture("res/images/fish/fishStoneBroken.png");
	fishStoneFracturedTexture = slLoadTexture("res/images/fish/fishStoneFractured.png");

	SetFishPositions(fish);
}

void DrawFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	for (int row = 0; row < MAX_ROW_FISH; row++)
	{
		for (int col = 0; col < MAX_COL_FISH; col++)
		{
			if (fish[row][col].isActive)
			{
				double x = fish[row][col].x;
				double y = fish[row][col].y;
				double width = fish[row][col].width;
				double height = fish[row][col].height;

				switch (fish[row][col].type)
				{
				case FishType::Normal:

					slSprite(normalFishTexture, x, y, width, height);

					break;
				case FishType::Fire:

					slSprite(fireFishTexture, x, y, width, height);

					break;
				case FishType::Speed:

					slSprite(speedFishTexture, x, y, width, height);

					break;
				case FishType::Slowness:

					slSprite(slownessFishTexture, x, y, width, height);

					break;
				case FishType::Rock:

					switch (fish[row][col].rockState)
					{
					case RockState::Fractured:

						slSprite(fishStoneFracturedTexture, x, y, width, height);

						break;
					case RockState::Broken:

						slSprite(fishStoneBrokenTexture, x, y, width, height);

						break;
					case RockState::Cracked:

						slSprite(fishStoneCrackedTexture, x, y, width, height);

						break;
					case RockState::Intact:

						slSprite(fishStoneIntactTexture, x, y, width, height);

						break;
					default:

						// THERE ARE NO MORE TYPES OF STONE FISH

						break;
					}
					break;

				default:

					// THERE ARE NO MORE TYPES OF FISH

					break;
				}
			}
		}
	}
}

static void SetFishPositions(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	double brickWidth = 90.0;
	double brickHeight = 50.0;
	double spacing = 5.0;
	double marginY = MARGIN_Y_BRICKS + 50;
	double totalWidth = MAX_COL_FISH * brickWidth + (MAX_COL_FISH - 1) * spacing;
	double offsetX = (SCREEN_WIDTH - totalWidth) / 2.0;

	for (int row = 0; row < MAX_ROW_FISH; row++)
	{
		for (int col = 0; col < MAX_COL_FISH; col++)
		{
			fish[row][col].width = brickWidth;
			fish[row][col].height = brickHeight;
			fish[row][col].x = offsetX + brickWidth / 2.0 + col * (brickWidth + spacing);
			fish[row][col].y = SCREEN_HEIGHT - (marginY + brickHeight / 2.0 + row * (brickHeight + spacing));
		}
	}
}

void ResetFish(Fish bricks[MAX_ROW_FISH][MAX_COL_FISH])
{
	for (int row = 0; row < MAX_ROW_FISH; row++)
	{
		for (int col = 0; col < MAX_COL_FISH; col++)
		{
			bricks[row][col].isActive = false;
			bricks[row][col].type = FishType::Empty;
			bricks[row][col].rockState = RockState::None;
		}
	}
}

static void SetRandomSpecialFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	for (int i = 0; i < MAX_FISH_SPECIALS; i++)
	{
		int row = 0;
		int col = 0;

		do
		{
			row = rand() % MAX_ROW_FISH;
			col = rand() % MAX_COL_FISH;
		} while (fish[row][col].isActive || fish[row][col].type != FishType::Empty);

		int specialType = static_cast<int>(FishType::Fire) + (rand() % TOTAL_FISH_SPECIALS);

		fish[row][col].type = static_cast<FishType>(specialType);
		fish[row][col].isActive = true;
	}
}

static void SetRandomRockFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	for (int i = 0; i < MAX_FISH_ROCK; i++)
	{
		int row = 0;
		int col = 0;

		do
		{
			row = rand() % MAX_ROW_FISH;
			col = rand() % MAX_COL_FISH;
		} while (fish[row][col].isActive || fish[row][col].type != FishType::Empty);

		fish[row][col].type = FishType::Rock;
		fish[row][col].rockState = RockState::Intact;
		fish[row][col].isActive = true;
	}
}

static void FillNormalFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	for (int row = 0; row < MAX_ROW_FISH; row++)
	{
		for (int col = 0; col < MAX_COL_FISH; col++)
		{
			if (!fish[row][col].isActive && fish[row][col].type == FishType::Empty)
			{
				fish[row][col].type = FishType::Normal;
				fish[row][col].isActive = true;
			}
		}
	}
}

void SetupFishTypes(Fish fish[MAX_ROW_FISH][MAX_COL_FISH])
{
	ResetFish(fish);
	SetRandomSpecialFish(fish);
	SetRandomRockFish(fish);
	FillNormalFish(fish);
}