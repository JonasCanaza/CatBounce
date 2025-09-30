#include "GameplayScreen.h"
#include "../utilities/Constants.h"
#include "../Game.h"
#include "../panel/PausePanel.h"
#include "../panel/GameOverPanel.h"

#include "sl.h"
#include <iostream>
#include <string>
#include <cmath>

namespace Gameplay
{
	bool isGameOver;

	int gameplayMusic;
	int gameplayMusicLoop;

	static int gameplayBackground;
	static int gameplayHUD;
	static int normalPalletteTexture;

	Pallette pall;
	Ball ball;
	Fish fish[MAX_ROW_FISH][MAX_COL_FISH];
	PowerItem powerItem[MAX_FISH_SPECIALS];

	static bool allBricksDestroyed = false;
	static double deltaTime;
	static double collisionCooldown = 0.0;

	// PRIVATE FUNCTIONS

	static bool CheckCollisionPalletteBall(Pallette pall, Ball ball);
	static bool CheckCollisionBallBrick(Ball ball, Fish brick);
	static bool HasActiveBricks(Fish bricks[MAX_ROW_FISH][MAX_COL_FISH]);
	static bool CheckCollisionPallettePowerItem(Pallette pall, PowerItem power);
	static bool IsPlayerAlive(Pallette pall);

	void Init()
	{
		isGameOver = false;

		gameplayMusic = slLoadWAV("res/music/gameplayMusic.wav");

		gameplayBackground = slLoadTexture("res/images/background/gameplay.png");
		gameplayHUD = slLoadTexture("res/images/ui/gameplayHUD.png");
		normalPalletteTexture = slLoadTexture("res/images/pallette/normalPallette.png");

		pall.width = 100.0;
		pall.height = 50.0;
		pall.x = SCREEN_WIDTH / 2.0;
		pall.y = pall.height;
		pall.speed = 700.0;
		pall.lives = 1;
		pall.score = 0;
		pall.isWinner = false;

		InitBall();
		InitFish(fish);
		InitPowerItems(powerItem);

		PausePanel::Init();
		GameOverPanel::Init();
	}

	void Input()
	{
		UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

		if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown)
		{
			slSoundPause(gameplayMusicLoop);
			PausePanel::isActive = !PausePanel::isActive;
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

			if (slGetKey(' '))
			{
				ball.isActive = true;
			}
		}
	}

	void Update()
	{
		deltaTime = slGetDeltaTime();

		if (collisionCooldown > 0.0)
		{
			collisionCooldown -= deltaTime;

			if (collisionCooldown <= 0.0)
			{
				collisionCooldown = 0.0;
			}
		}

		if (!PausePanel::isActive && !isGameOver)
		{
			if (pall.x - pall.width / 2.0 < 0)
			{
				pall.x = pall.width / 2.0;
			}
			if (pall.x + pall.width / 2.0 > SCREEN_WIDTH)
			{
				pall.x = SCREEN_WIDTH - pall.width / 2;
			}

			if (ball.isActive)
			{
				ball.y += ball.speedY * deltaTime;
				ball.x += ball.speedX * deltaTime;
				
				if (ball.type != BallType::Normal)
				{
					ball.durationEffect -= deltaTime;
				}
			}
			else
			{
				//ball.x = pall.x;
				//ball.y = pall.y + pall.height + ball.radius;

				//BALL TEST!!!
				if (slGetKey(SL_KEY_UP))
				{
					ball.y += 300 * deltaTime;
				}
				if (slGetKey(SL_KEY_LEFT))
				{
					ball.x -= 300 * deltaTime;
				}
				if (slGetKey(SL_KEY_DOWN))
				{
					ball.y -= 300 * deltaTime;
				}
				if (slGetKey(SL_KEY_RIGHT))
				{
					ball.x += 300 * deltaTime;
				}
			}

			if (ball.x - ball.radius < 0)
			{
				ball.x = ball.radius;
				ball.speedX *= -1.0;
				PlayDefaultHitSound();
			}
			if (ball.x + ball.radius > SCREEN_WIDTH)
			{
				ball.x = SCREEN_WIDTH - ball.radius;
				ball.speedX *= -1.0;
				PlayDefaultHitSound();
			}

			if (ball.y + ball.radius > MAXIMUM_TOP_Y)
			{
				ball.y = MAXIMUM_TOP_Y - ball.radius;
				ball.speedY *= -1.0;
				PlayDefaultHitSound();
			}
			if (ball.y + ball.radius < 0)
			{
				ball.speedY = std::abs(ball.speedY);
				pall.lives--;
				ResetBall();

				if (pall.lives <= 0)
				{
					isGameOver = true;
					GameOverPanel::isActive = true;
					pall.isWinner = false;
				}
			}

			for (int row = 0; row < MAX_ROW_FISH; row++)
			{
				for (int col = 0; col < MAX_COL_FISH; col++)
				{
					if (fish[row][col].isActive && CheckCollisionBallBrick(ball, fish[row][col]))
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
										//std::cout << "LEFT TOP collision" << std::endl;
										ball.speedY = fabs(ball.speedY);
									}
									else
									{
										//std::cout << "LEFT BOTTOM collision" << std::endl;
										ball.speedY = -fabs(ball.speedY);
									}
								}
								else // RIGHT
								{
									ball.x = fish[row][col].x + fish[row][col].width / 2.0 + ball.radius + 0.1;
									ball.speedX = fabs(ball.speedX);

									if (ball.y > fish[row][col].y)
									{
										//std::cout << "RIGHT TOP collision" << std::endl;
										ball.speedY = fabs(ball.speedY);
									}
									else
									{
										//std::cout << "RIGHT BOTTOM collision" << std::endl;
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
										//std::cout << "TOP LEFT collision" << std::endl;
										ball.speedX = -fabs(ball.speedX);
									}
									else
									{
										//std::cout << "TOP RIGHT collision" << std::endl;
										ball.speedX = fabs(ball.speedX);
									}
								}
								else // BOTTOM
								{
									ball.y = fish[row][col].y - fish[row][col].height / 2.0 - ball.radius - 0.1;
									ball.speedY = -fabs(ball.speedY);

									if (ball.x < fish[row][col].x)
									{
										//std::cout << "BOTTOM LEFT collision" << std::endl;
										ball.speedX = -fabs(ball.speedX);
									}
									else
									{
										//std::cout << "BOTTOM RIGHT collision" << std::endl;
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
									pall.score += 50;
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

			if (CheckCollisionPalletteBall(pall, ball) && collisionCooldown <= 0.0)
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
							//std::cout << "Left TOP!" << std::endl;
						}
						else
						{
							//std::cout << "Left BUTTOM!" << std::endl;
						}
					}
					else // RIGHT
					{
						ball.x = pall.x + pall.width / 2.0 + ball.radius + 0.1;
						ball.speedX = fabs(ball.speedX);

						if (ball.y > pall.y)
						{
							ball.speedY *= -1.0;
							//std::cout << "Right TOP!" << std::endl;
						}
						else
						{
							//std::cout << "Right BUTTOM!" << std::endl;
						}
					}
				}
				else // TOP
				{
					ball.y = pall.y + pall.height / 2.0 + ball.radius + 0.1;
					ball.speedY *= -1.0;
					//std::cout << "Top!" << std::endl;
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

			for (int i = 0; i < MAX_FISH_SPECIALS; i++)
			{
				if (powerItem[i].isActive && CheckCollisionPallettePowerItem(pall, powerItem[i]))
				{
					switch (powerItem[i].type)
					{
					case PowerItemType::Fire:

						PlaySpellSound(PowerItemType::Fire);
						ball.type = BallType::Fire;
						std::cout << "Fire power-up collected!" << std::endl;

						break;
					case PowerItemType::Speed:

						PlaySpellSound(PowerItemType::Speed);
						std::cout << "Speed power-up collected!" << std::endl;
						pall.speed *= 1.1;

						break;
					case PowerItemType::Slowness:

						PlaySpellSound(PowerItemType::Slowness);
						std::cout << "Slowness power-down collected!" << std::endl;
						pall.speed *= 0.9;

						break;
					case PowerItemType::Life:

						PlaySpellSound(PowerItemType::Life);
						std::cout << "Life power-up collected!" << std::endl;
						pall.lives++;

						break;
					case PowerItemType::Water:

						PlaySpellSound(PowerItemType::Water);
						std::cout << "Water power-down collected!" << std::endl;
						ball.type = BallType::Water;

						break;
					case PowerItemType::Poison:

						PlaySpellSound(PowerItemType::Poison);
						std::cout << "Poison power-down collected!" << std::endl;
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

			if (!HasActiveBricks(fish))
			{
				isGameOver = true;
				GameOverPanel::isActive = true;
				pall.isWinner = true;
			}

			UpdatePowerItems(powerItem, deltaTime);
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

		DrawPowerItems(powerItem);

		DrawFish(fish);

		slSprite(normalPalletteTexture, pall.x, pall.y, pall.width, pall.height);

		slSetForeColor(0.0, 0.0, 1.0, 1.0);
		slRectangleOutline(pall.x, pall.y, pall.width, pall.height);

		DrawBall();

		slSetForeColor(1.0, 0.0, 0.0, 1.0);
		slRectangleOutline(ball.x, ball.y, ball.radius * 2.0, ball.radius * 2.0);

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

	static bool CheckCollisionPalletteBall(Pallette pall, Ball ball)
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

	static bool CheckCollisionBallBrick(Ball ball, Fish brick)
	{
		double leftBall = ball.x - ball.radius;
		double rightBall = ball.x + ball.radius;
		double topBall = ball.y + ball.radius;
		double bottomBall = ball.y - ball.radius;

		double leftBrick = brick.x - brick.width / 2.0;
		double rightBrick = brick.x + brick.width / 2.0;
		double topBrick = brick.y + brick.height / 2.0;
		double bottomBrick = brick.y - brick.height / 2.0;

		if (rightBrick < leftBall ||
			leftBrick > rightBall ||
			topBrick < bottomBall ||
			bottomBrick > topBall)
		{
			return false;
		}

		return true;
	}

	static bool HasActiveBricks(Fish bricks[MAX_ROW_FISH][MAX_COL_FISH])
	{
		for (int row = 0; row < MAX_ROW_FISH; row++)
		{	
			for (int col = 0; col < MAX_COL_FISH; col++)
			{
				if (bricks[row][col].isActive)
				{
					return true;
				}
			}
		}

		return false;
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

	static bool IsPlayerAlive(Pallette pall)
	{
		if (pall.lives <= 0)
		{
			return false;
		}

		return true;
	}

	void ResetBall()
	{
		ball.x = SCREEN_WIDTH / 2.0;
		ball.y = SCREEN_HEIGHT / 2.0;
		ball.speedX = 300.0;
		ball.speedY = 325.0;
		ball.isActive = false;
	}
}