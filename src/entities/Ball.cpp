#include "Ball.h"

#include "sl.h"
#include <ctime>
#include <stdlib.h>

static int ballHitSound1;
static int ballHitSound2;
static int ballHitSound3;
static int ballHitSound4;

void InitBall()
{
	ballHitSound1 = slLoadWAV("res/sound/ball/ballHit01.wav");
	ballHitSound2 = slLoadWAV("res/sound/ball/ballHit02.wav");
	ballHitSound3 = slLoadWAV("res/sound/ball/ballHit03.wav");
	ballHitSound4 = slLoadWAV("res/sound/ball/ballHit04.wav");
}

void PlayImpactSound()
{
	switch (rand() % MAX_BALL_HITS)
	{
	case 0:

		slSoundPlay(ballHitSound1);

		break;
	case 1:

		slSoundPlay(ballHitSound2);

		break;
	case 2:

		slSoundPlay(ballHitSound3);

		break;
	case 3:

		slSoundPlay(ballHitSound4);

		break;
	default:

		// NO MORE KNOCKING SOUNDS

		break;
	}
}
