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
void UpdateBall(double deltaTime);
void DrawBall();
void SetBallDefault();
void SetBallDefaultPosition();
void UpdateEffectTimer(double deltaTime);
void PlayDefaultHitSound();
void PlayFishBreakSound();
void PlayRockHitSound();
void PlayRockBreakSound();
void PlayFishSpecialSound();