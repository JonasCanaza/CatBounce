#include "PowerItem.h"

#include "sl.h"

static int fireTexture;
static int speedTexture;
static int slownessTexture;

static void SetPowerItemsDefaults(PowerItem powerItem[MAX_FISH_SPECIALS]);

void InitPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS])
{
    fireTexture = slLoadTexture("res/images/spell/fire.png");
    speedTexture = slLoadTexture("res/images/spell/speed.png");
    slownessTexture = slLoadTexture("res/images/spell/slowness.png");

    SetPowerItemsDefaults(powerItem);
}

static void SetPowerItemsDefaults(PowerItem powerItem[MAX_FISH_SPECIALS])
{
	double width = 40.0;
	double height = 40.0;
	double speedY = 200.0;

	for (int i = 0; i < MAX_FISH_SPECIALS; i++)
	{
		powerItem[i].width = width;
		powerItem[i].height = height;
		powerItem[i].speedY = speedY;
        powerItem[i].isActive = false;
        powerItem[i].type = PowerItemType::None;
	}
}

void ResetPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS])
{
    for (int i = 0; i < MAX_FISH_SPECIALS; i++)
    {
        powerItem[i].isActive = false;
        powerItem[i].type = PowerItemType::None;
    }
}

void SpawnPowerItem(PowerItem powerItem[MAX_FISH_SPECIALS], double spawnX, double spawnY, PowerItemType type)
{
    for (int i = 0; i < MAX_FISH_SPECIALS; i++)
    {
        if (!powerItem[i].isActive)
        {
            powerItem[i].x = spawnX;
            powerItem[i].y = spawnY;
            powerItem[i].type = type;
            powerItem[i].isActive = true;

            break;
        }
    }
}

void UpdatePowerItems(PowerItem powerItem[MAX_FISH_SPECIALS], double deltaTime)
{
    for (int i = 0; i < MAX_FISH_SPECIALS; i++)
    {
        if (powerItem[i].isActive)
        {
            powerItem[i].y -= powerItem[i].speedY * deltaTime;

            if (powerItem[i].y + powerItem[i].height / 2 < 0)
            {
                powerItem[i].isActive = false;
                powerItem[i].type = PowerItemType::None;
            }
        }
    }
}

void DrawPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS])
{
    for (int i = 0; i < MAX_FISH_SPECIALS; i++)
    {
        if (powerItem[i].isActive)
        {
            int texture = 0;

            switch (powerItem[i].type)
            {
            case PowerItemType::Fire:
                
                texture = fireTexture;
                
                break;
            case PowerItemType::Speed:
                
                texture = speedTexture;

                break;
            case PowerItemType::Slowness:
                
                texture = slownessTexture;
                
                break;
            default:

                // THERE ARE NO MORE TYPES OF SPECIAL FISH

                break;
            }

            slSetForeColor(1.0, 1.0, 1.0, 1.0);
            slSprite(texture, powerItem[i].x, powerItem[i].y, powerItem[i].width, powerItem[i].height);
            // TEST!!
            slSetForeColor(1.0, 0.0, 0.0, 1.0);
            slRectangleOutline(powerItem[i].x, powerItem[i].y, powerItem[i].width, powerItem[i].height);
            slSetForeColor(1.0, 1.0, 1.0, 1.0);
        }
    }
}