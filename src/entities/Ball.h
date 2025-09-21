#pragma once

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