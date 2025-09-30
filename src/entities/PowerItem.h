#pragma once

const int MAX_FISH_SPECIALS = 5;

enum class PowerItemType
{
    None,
    Fire,
    Speed,
    Slowness,
    Life,
    Water,
    Poison
};

struct PowerItem
{
    double width;
    double height;
    double x;
    double y;
    double speedY;
    bool isActive;
    PowerItemType type;
};

void InitPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS]);
void UpdatePowerItems(PowerItem powerItem[MAX_FISH_SPECIALS], double deltaTime);
void DrawPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS]);
void ResetPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS]);
void SpawnPowerItem(PowerItem powerItem[MAX_FISH_SPECIALS], double spawnX, double spawnY, PowerItemType type);
void PlaySpellSound(PowerItemType type);