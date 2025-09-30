#include "GameplayScreen.h"
#include "../Game.h"
#include "../utilities/Constants.h"
#include "../panel/PausePanel.h"
#include "../panel/GameOverPanel.h"

#include "sl.h"
#include <iostream>
#include <string>
#include <cmath>

namespace Gameplay
{
	bool isGameOver;

	static int gameplayBackground;
	static int gameplayHUD;

	int gameplayMusic;
	int gameplayMusicLoop;

	Pallette pall;
	Ball ball;
	Fish fish[MAX_ROW_FISH][MAX_COL_FISH];
	PowerItem powerItem[MAX_FISH_SPECIALS];

	static bool allBricksDestroyed = false;
	static double deltaTime;
	static double collisionCooldown = 0.0;

	// PRIVATE FUNCTIONS

	static void UpdateNoCollisionTimePallBall();
	static bool CheckCollisionPalletteBall();
	static bool CheckCollisionBallFish(Ball ball, Fish brick);
	static bool CheckCollisionPallettePowerItem(Pallette pall, PowerItem power);
	static void HandleBallFishCollisions();
	static void HandleBallPalletteCollision();
	static void HandlePallettePowerItemCollisions();
	static bool HasActiveFish();
	static bool IsPlayerAlive();

	void Init()
	{
		isGameOver = false;

		gameplayBackground = slLoadTexture("res/images/background/gameplay.png");
		gameplayHUD = slLoadTexture("res/images/ui/gameplayHUD.png");

		gameplayMusic = slLoadWAV("res/music/gameplayMusic.wav");

		InitPallette();
		InitBall();
		InitFish(fish);
		InitPowerItems(powerItem);

		PausePanel::Init();
		GameOverPanel::Init();
	}

	void Input()
	{
		UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

		if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown && !isGameOver)
		{
			slSoundPause(gameplayMusicLoop);	
			PausePanel::isActive = !PausePanel::isActive;

			if (!PausePanel::isActive)
			{
				slSoundResumeAll();
			}
		}

		if (!PausePanel::isActive && !isGameOver)
		{
			if (slGetKey('a') || slGetKey('A') || slGetKey(SL_KEY_LEFT))
			{
				pall.x -= pall.speed * deltaTime;
			}

			if (slGetKey('d') || slGetKey('D') || slGetKey(SL_KEY_RIGHT))
			{
				pall.x += pall.speed * deltaTime;
			}

			if (slGetKey(' ') || slGetKey(SL_KEY_UP))
			{
				ball.isActive = true;
			}
		}
	}

	void Update()
	{
		deltaTime = slGetDeltaTime();

		if (!PausePanel::isActive && !isGameOver)
		{
			UpdateNoCollisionTimePallBall();
			UpdateEffectTimer(deltaTime);

			UpdatePallete();
			UpdateBall(deltaTime);

			HandleBallFishCollisions();
			HandleBallPalletteCollision();
			HandlePallettePowerItemCollisions();

			UpdatePowerItems(powerItem, deltaTime);

			if (!HasActiveFish())
			{
				isGameOver = true;
				GameOverPanel::isActive = true;
				pall.isWinner = true;
			}

			if (!IsPlayerAlive())
			{
				isGameOver = true;
				GameOverPanel::isActive = true;
				pall.isWinner = false;
			}
		}
		else
		{
			if (PausePanel::isActive)
			{
				PausePanel::Update();
			}

			if (GameOverPanel::isActive)
			{
				GameOverPanel::Update();
			}
		}
	}

	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSprite(gameplayBackground, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);

		DrawFish(fish);
		DrawPowerItems(powerItem);
		DrawPallete();
		DrawBall();

		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSprite(gameplayHUD, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT - 50.0, SCREEN_WIDTH, 100.0);

		slSetFont(CatBounce::specialFont, 40);

		std::string textLives = "Lives: " + std::to_string(pall.lives);
		double livesHeight = slGetTextHeight(textLives.c_str());
		slText(50, SCREEN_HEIGHT - 50.0 - livesHeight / 2.0, textLives.c_str());

		std::string textScore = "Score: " + std::to_string(pall.score);
		double scoreWidth = slGetTextWidth(textScore.c_str());
		double scoreHeight = slGetTextHeight(textScore.c_str());
		slText(SCREEN_WIDTH - scoreWidth - 50, SCREEN_HEIGHT - 50.0 - scoreHeight / 2.0, textScore.c_str());

		if (PausePanel::isActive)
		{
			PausePanel::Draw();
		}

		if (GameOverPanel::isActive)
		{
			GameOverPanel::Draw();
		}

		slRender();
	}

	static void UpdateNoCollisionTimePallBall()
	{
		if (collisionCooldown > 0.0)
		{
			collisionCooldown -= deltaTime;

			if (collisionCooldown <= 0.0)
			{
				collisionCooldown = 0.0;
			}
		}
	}

	static bool CheckCollisionPalletteBall()
	{
		double leftPall = pall.x - pall.width / 2.0;
		double rightPall = pall.x + pall.width / 2.0;
		double topPall = pall.y + pall.height / 2.0;
		double bottomPall = pall.y - pall.height / 2.0;

		double leftBall = ball.x - ball.radius;
		double rightBall = ball.x + ball.radius;
		double topBall = ball.y + ball.radius;
		double bottomBall = ball.y - ball.radius;

		if (rightPall < leftBall ||
			leftPall > rightBall ||
			topPall < bottomBall ||
			bottomPall > topBall)
		{
			return false;
		}

		return true;
	}

	static bool CheckCollisionBallFish(Ball ball, Fish fish)
	{
		double leftBall = ball.x - ball.radius;
		double rightBall = ball.x + ball.radius;
		double topBall = ball.y + ball.radius;
		double bottomBall = ball.y - ball.radius;

		double leftBrick = fish.x - fish.width / 2.0;
		double rightBrick = fish.x + fish.width / 2.0;
		double topBrick = fish.y + fish.height / 2.0;
		double bottomBrick = fish.y - fish.height / 2.0;

		if (rightBrick < leftBall ||
			leftBrick > rightBall ||
			topBrick < bottomBall ||
			bottomBrick > topBall)
		{
			return false;
		}

		return true;
	}

	static bool CheckCollisionPallettePowerItem(Pallette pall, PowerItem power)
	{
		double leftPall = pall.x - pall.width / 2.0;
		double rightPall = pall.x + pall.width / 2.0;
		double topPall = pall.y + pall.height / 2.0;
		double bottomPall = pall.y - pall.height / 2.0;

		double leftPower = power.x - power.width / 2.0;
		double rightPower = power.x + power.width / 2.0;
		double topPower = power.y + power.height / 2.0;
		double bottomPower = power.y - power.height / 2.0;

		if (rightPall < leftPower ||
			leftPall > rightPower ||
			topPall < bottomPower ||
			bottomPall > topPower)
		{
			return false;
		}

		return true;
	}

	static void HandleBallFishCollisions()
	{
		for (int row = 0; row < MAX_ROW_FISH; row++)
		{
			for (int col = 0; col < MAX_COL_FISH; col++)
			{
				if (fish[row][col].isActive && CheckCollisionBallFish(ball, fish[row][col]))
				{
					if (ball.type == BallType::Normal || ball.type == BallType::Water)
					{
						double deltaX = ball.x - fish[row][col].x;
						double deltaY = ball.y - fish[row][col].y;

						double overlapHorizontal = (fish[row][col].width / 2.0 + ball.radius) - fabs(deltaX);
						double overlapVertical = (fish[row][col].height / 2.0 + ball.radius) - fabs(deltaY);

						if (overlapHorizontal < overlapVertical)
						{
							if (deltaX < 0) // LEFT
							{
								ball.x = fish[row][col].x - fish[row][col].width / 2.0 - ball.radius - 0.1;
								ball.speedX = -fabs(ball.speedX);

								if (ball.y > fish[row][col].y)
								{
									ball.speedY = fabs(ball.speedY);
								}
								else
								{
									ball.speedY = -fabs(ball.speedY);
								}
							}
							else // RIGHT
							{
								ball.x = fish[row][col].x + fish[row][col].width / 2.0 + ball.radius + 0.1;
								ball.speedX = fabs(ball.speedX);

								if (ball.y > fish[row][col].y)
								{
									ball.speedY = fabs(ball.speedY);
								}
								else
								{
									ball.speedY = -fabs(ball.speedY);
								}
							}
						}
						else
						{
							if (deltaY > 0) // TOP
							{
								ball.y = fish[row][col].y + fish[row][col].height / 2.0 + ball.radius + 0.1;
								ball.speedY = fabs(ball.speedY);

								if (ball.x < fish[row][col].x)
								{
									ball.speedX = -fabs(ball.speedX);
								}
								else
								{
									ball.speedX = fabs(ball.speedX);
								}
							}
							else // BOTTOM
							{
								ball.y = fish[row][col].y - fish[row][col].height / 2.0 - ball.radius - 0.1;
								ball.speedY = -fabs(ball.speedY);

								if (ball.x < fish[row][col].x)
								{
									ball.speedX = -fabs(ball.speedX);
								}
								else
								{
									ball.speedX = fabs(ball.speedX);
								}
							}
						}
					}

					if (ball.type != BallType::Water)
					{
						switch (fish[row][col].type)
						{
						case FishType::Normal:

							PlayFishBreakSound();
							pall.score += 100;
							fish[row][col].isActive = false;

							break;
						case FishType::Fire:
						case FishType::Speed:
						case FishType::Slowness:
						case FishType::Life:
						case FishType::Water:
						case FishType::Poison:

							PlayFishSpecialSound();
							pall.score += 125;
							fish[row][col].isActive = false;

							switch (fish[row][col].type)
							{
							case FishType::Fire:

								SpawnPowerItem(powerItem, fish[row][col].x, fish[row][col].y, PowerItemType::Fire);

								break;
							case FishType::Speed:

								SpawnPowerItem(powerItem, fish[row][col].x, fish[row][col].y, PowerItemType::Speed);

								break;
							case FishType::Slowness:

								SpawnPowerItem(powerItem, fish[row][col].x, fish[row][col].y, PowerItemType::Slowness);

								break;
							case FishType::Life:

								SpawnPowerItem(powerItem, fish[row][col].x, fish[row][col].y, PowerItemType::Life);

								break;
							case FishType::Water:

								SpawnPowerItem(powerItem, fish[row][col].x, fish[row][col].y, PowerItemType::Water);

								break;
							case FishType::Poison:

								SpawnPowerItem(powerItem, fish[row][col].x, fish[row][col].y, PowerItemType::Poison);

								break;
							default:

								// THERE ARE NO MORE TYPES OF SPECIAL FISH

								break;
							}

							break;
						case FishType::Rock:

							switch (fish[row][col].rockState)
							{
							case RockState::Intact:
							case RockState::Cracked:
							case RockState::Broken:

								PlayRockHitSound();
								pall.score += 25;
								fish[row][col].rockState = static_cast<RockState>(static_cast<int>(fish[row][col].rockState) - 1);

								break;
							case RockState::Fractured:

								PlayRockBreakSound();
								pall.score += 150;
								fish[row][col].isActive = false;

								break;
							default:

								// THERE ARE NO MORE TYPES OF STONE FISH

								break;
							}

							break;
						default:

							// THERE ARE NO MORE TYPES OF FISH

							break;
						}
					}
					else
					{
						PlayDefaultHitSound();
					}
				}
			}
		}
	}

	static void HandleBallPalletteCollision()
	{
		if (CheckCollisionPalletteBall() && collisionCooldown <= 0.0)
		{
			double deltaX = ball.x - pall.x;
			double deltaY = ball.y - pall.y;

			double overlapHorizontal = (pall.width / 2.0 + ball.radius) - fabs(deltaX);
			double overlapVertical = (pall.height / 2.0 + ball.radius) - fabs(deltaY);

			if (overlapHorizontal < overlapVertical)
			{
				if (deltaX < 0) // LEFT
				{
					ball.x = pall.x - pall.width / 2.0 - ball.radius - 0.1;
					ball.speedX = -fabs(ball.speedX);

					if (ball.y > pall.y)
					{
						ball.speedY *= -1.0;
					}
				}
				else // RIGHT
				{
					ball.x = pall.x + pall.width / 2.0 + ball.radius + 0.1;
					ball.speedX = fabs(ball.speedX);

					if (ball.y > pall.y)
					{
						ball.speedY *= -1.0;
					}
				}
			}
			else // TOP
			{
				ball.y = pall.y + pall.height / 2.0 + ball.radius + 0.1;
				ball.speedY *= -1.0;
			}

			double relativeImpact = (ball.x - pall.x) / (pall.width / 2.0);

			if (relativeImpact < -1.0)
			{
				relativeImpact = -1.0;
			}

			if (relativeImpact > 1.0)
			{
				relativeImpact = 1.0;
			}

			double maxDeviation = 400.0;
			ball.speedX = maxDeviation * relativeImpact;

			collisionCooldown = 0.75;

			PlayDefaultHitSound();
		}
	}

	static void HandlePallettePowerItemCollisions()
	{
		for (int i = 0; i < MAX_FISH_SPECIALS; i++)
		{
			if (powerItem[i].isActive && CheckCollisionPallettePowerItem(pall, powerItem[i]))
			{
				switch (powerItem[i].type)
				{
				case PowerItemType::Fire:

					PlaySpellSound(PowerItemType::Fire);
					ball.type = BallType::Fire;
					ball.durationEffect = 6.5;

					break;
				case PowerItemType::Speed:

					PlaySpellSound(PowerItemType::Speed);
					pall.speed *= 1.15;

					break;
				case PowerItemType::Slowness:

					PlaySpellSound(PowerItemType::Slowness);
					pall.speed *= 0.85;

					break;
				case PowerItemType::Life:

					PlaySpellSound(PowerItemType::Life);
					pall.lives++;

					break;
				case PowerItemType::Water:

					PlaySpellSound(PowerItemType::Water);
					ball.type = BallType::Water;
					ball.durationEffect = 15.0;

					break;
				case PowerItemType::Poison:

					PlaySpellSound(PowerItemType::Poison);
					pall.lives--;

					break;
				default:

					// THERE ARE NO MORE TYPES OF POWER ITEMS

					break;
				}

				powerItem[i].isActive = false;
				powerItem[i].type = PowerItemType::None;
			}
		}
	}

	static bool HasActiveFish()
	{
		for (int row = 0; row < MAX_ROW_FISH; row++)
		{	
			for (int col = 0; col < MAX_COL_FISH; col++)
			{
				if (fish[row][col].isActive)
				{
					return true;
				}
			}
		}

		return false;
	}

	static bool IsPlayerAlive()
	{
		if (pall.lives <= 0)
		{
			return false;
		}

		return true;
	}

	void ResetLevel()
	{
		SetPalleteDefault();
		SetBallDefault();
		SetupFishTypes(fish);
		ResetPowerItems(powerItem);
	}
}