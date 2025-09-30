#include "PowerItem.h"

#include "sl.h"

// TEXTURES

static int fireTexture;
static int speedTexture;
static int slownessTexture;
static int lifeTexture;
static int waterTexture;
static int poisonTexture;

// SOUNDS

static int fireSound;
static int speedSound;
static int slownessSound;
static int lifeSound;
static int waterSound;
static int poisonSound;

static void SetPowerItemsDefaults(PowerItem powerItem[MAX_FISH_SPECIALS]);

void InitPowerItems(PowerItem powerItem[MAX_FISH_SPECIALS])
{
    fireTexture = slLoadTexture("res/images/spell/fire.png");
    speedTexture = slLoadTexture("res/images/spell/speed.png");
    slownessTexture = slLoadTexture("res/images/spell/slowness.png");
    lifeTexture = slLoadTexture("res/images/spell/life.png");
    waterTexture = slLoadTexture("res/images/spell/water.png");
    poisonTexture = slLoadTexture("res/images/spell/poison.png");

    fireSound = slLoadWAV("res/sound/spell/fire.wav");
    speedSound = slLoadWAV("res/sound/spell/speed.wav");
    slownessSound = slLoadWAV("res/sound/spell/slow.wav");
    lifeSound = slLoadWAV("res/sound/spell/life.wav");
    waterSound = slLoadWAV("res/sound/spell/water.wav");
    poisonSound = slLoadWAV("res/sound/spell/poison.wav");

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
            case PowerItemType::Life:

                texture = lifeTexture;

                break;
            case PowerItemType::Water:

                texture = waterTexture;

                break;
            case PowerItemType::Poison:
                
                texture = poisonTexture;

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

void PlaySpellSound(PowerItemType type)
{
    switch (type)
    {
    case PowerItemType::Fire:

        slSoundPlay(fireSound);

        break;
    case PowerItemType::Speed:

        slSoundPlay(speedSound);

        break;
    case PowerItemType::Slowness:

        slSoundPlay(slownessSound);

        break;
    case PowerItemType::Life:

        slSoundPlay(lifeSound);

        break;
    case PowerItemType::Water:

        slSoundPlay(waterSound);

        break;
    case PowerItemType::Poison:

        slSoundPlay(poisonSound);

        break;
    default:

        // THERE ARE NO MORE TYPES OF SPELLS

        break;
    }
}