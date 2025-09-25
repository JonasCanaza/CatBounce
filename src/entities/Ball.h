#pragma once

const int MAX_BALL_HITS = 4;

struct Ball
{
	double radius;
	double x;
	double y;
	double speedX;
	double speedY;
	bool isActive;
};

void InitBall();
void PlayImpactSound();