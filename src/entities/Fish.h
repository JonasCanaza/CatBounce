#pragma once

const int MAX_COL_FISH = 7;
const int MAX_ROW_FISH = 5;

enum class FishType
{
	Empty,
	Normal,
	Fire,
	Speed,
	Slowness,
	Rock
};

enum class RockState
{
	None,
	Fractured,
	Broken,
	Cracked,
	Intact
};

struct Fish
{
	double width;
	double height;
	double x;
	double y;
	bool isActive;
	FishType type;
	RockState rockState = RockState::None;
};

void InitFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH]);
void DrawFish(Fish fish[MAX_ROW_FISH][MAX_COL_FISH]);
void SetupFishTypes(Fish fish[MAX_ROW_FISH][MAX_COL_FISH]);
int GetSpecialFishQuantity();