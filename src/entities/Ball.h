#pragma once

enum class BallType
{
	Normal,
	Fire,
	Water
};

struct Ball
{
	double radius;
	double x;
	double y;
	double speedX;
	double speedY;
	bool isActive;
	BallType type;
	double durationEffect;
};

void InitBall();
void DrawBall();
void SetBallDefault();
void SetBallDefaultPosition();
void PlayDefaultHitSound();
void PlayFishBreakSound();
void PlayRockHitSound();
void PlayRockBreakSound();
void PlayFishSpecialSound();