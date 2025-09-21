#include "Ball.h"

#include "sl.h"

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
	slSoundPlay(ballHitSound1);
}
