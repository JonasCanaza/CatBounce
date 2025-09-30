#include "Ball.h"
#include "../screen/GameplayScreen.h"
#include "../utilities/Constants.h"

#include "sl.h"
#include <ctime>
#include <stdlib.h>

static const int MAX_EDGE_HITS = 4;
static const int MAX_FISH_BREAK = 4;
static const int MAX_ROCK_HITS = 3;
static const int MAX_ROCK_BREAK = 3;

// TEXTURES

static int normalBallTexture;
static int fireBallTexture;
static int waterBallTexture;

// SOUNDS

static int edgeHit1;
static int edgeHit2;
static int edgeHit3;
static int edgeHit4;
static int fishBreak1;
static int fishBreak2;
static int fishBreak3;
static int fishBreak4;
static int rockHit1;
static int rockHit2;
static int rockHit3;
static int rockBreak1;
static int rockBreak2;
static int rockBreak3;
static int fishSpecialHit;

void InitBall()
{
	normalBallTexture = slLoadTexture("res/images/ball/normalBall.png");
	fireBallTexture = slLoadTexture("res/images/ball/fireBall.png");
	waterBallTexture = slLoadTexture("res/images/ball/waterBall.png");

	edgeHit1 = slLoadWAV("res/sound/ball/edgeHit/hit01.wav");
	edgeHit2 = slLoadWAV("res/sound/ball/edgeHit/hit02.wav");
	edgeHit3 = slLoadWAV("res/sound/ball/edgeHit/hit03.wav");
	edgeHit4 = slLoadWAV("res/sound/ball/edgeHit/hit04.wav");

	fishBreak1 = slLoadWAV("res/sound/ball/normalBreakingFish/hit01.wav");
	fishBreak2 = slLoadWAV("res/sound/ball/normalBreakingFish/hit02.wav");
	fishBreak3 = slLoadWAV("res/sound/ball/normalBreakingFish/hit03.wav");
	fishBreak4 = slLoadWAV("res/sound/ball/normalBreakingFish/hit04.wav");

	rockHit1 = slLoadWAV("res/sound/ball/fishRockHit/hit01.wav");
	rockHit2 = slLoadWAV("res/sound/ball/fishRockHit/hit02.wav");
	rockHit3 = slLoadWAV("res/sound/ball/fishRockHit/hit03.wav");

	rockBreak1 = slLoadWAV("res/sound/ball/fishRockBreaking/hit01.wav");
	rockBreak2 = slLoadWAV("res/sound/ball/fishRockBreaking/hit02.wav");
	rockBreak3 = slLoadWAV("res/sound/ball/fishRockBreaking/hit03.wav");

	fishSpecialHit = slLoadWAV("res/sound/ball/specialBreakingFish/hit01.wav");

	SetBallDefault();
}

void DrawBall()
{
	int texture = 0;

	switch (Gameplay::ball.type)
	{
	case BallType::Normal:

		texture = normalBallTexture;

		break;
	case BallType::Fire:

		texture = fireBallTexture;

		break;
	case BallType::Water:

		texture = waterBallTexture;

		break;
	default:

		// THERE ARE NO MORE TYPES OF BALLS

		break;
	}

	slSetForeColor(1.0, 1.0, 1.0, 1.0);
	slSprite(texture, Gameplay::ball.x, Gameplay::ball.y, Gameplay::ball.radius * 2.0, Gameplay::ball.radius * 2.0);
}

void SetBallDefault()
{
	SetBallDefaultPosition();
	Gameplay::ball.speedX = 300.0;
	Gameplay::ball.speedY = 325.0;
	Gameplay::ball.type = BallType::Normal;
	Gameplay::ball.durationEffect = 0.0;
}

void SetBallDefaultPosition()
{
	Gameplay::ball.radius = 15.0;
	Gameplay::ball.x = Gameplay::pall.x;
	Gameplay::ball.y = Gameplay::pall.y + Gameplay::pall.height / 2.0 + Gameplay::ball.radius + 0.1;
	Gameplay::ball.isActive = false;
}

void PlayDefaultHitSound()
{
	switch (rand() % MAX_EDGE_HITS)
	{
	case 0:

		slSoundPlay(edgeHit1);

		break;
	case 1:

		slSoundPlay(edgeHit2);

		break;
	case 2:

		slSoundPlay(edgeHit3);

		break;
	case 3:

		slSoundPlay(edgeHit4);

		break;
	default:

		// NO MORE SOUNDS

		break;
	}
}

void PlayFishBreakSound()
{
	switch (rand() % MAX_FISH_BREAK)
	{
	case 0:

		slSoundPlay(fishBreak1);

		break;
	case 1:

		slSoundPlay(fishBreak2);

		break;
	case 2:

		slSoundPlay(fishBreak3);

		break;
	case 3:

		slSoundPlay(fishBreak4);

		break;
	default:

		// NO MORE SOUNDS

		break;
	}
}

void PlayRockHitSound()
{
	switch (rand() % MAX_ROCK_HITS)
	{
	case 0:

		slSoundPlay(rockHit1);

		break;
	case 1:

		slSoundPlay(rockHit2);

		break;
	case 2:

		slSoundPlay(rockHit3);

		break;
	default:

		// NO MORE SOUNDS

		break;
	}
}

void PlayRockBreakSound()
{
	switch (rand() % MAX_ROCK_BREAK)
	{
	case 0:

		slSoundPlay(rockBreak1);

		break;
	case 1:

		slSoundPlay(rockBreak2);

		break;
	case 2:

		slSoundPlay(rockBreak3);

		break;
	default:

		// NO MORE SOUNDS

		break;
	}
}

void PlayFishSpecialSound()
{
	slSoundPlay(fishSpecialHit);
}