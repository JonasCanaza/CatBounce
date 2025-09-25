#include "GameplayScreen.h"
#include "../entities/Pallette.h"
#include "../entities/Ball.h"
#include "../entities/Brick.h"
#include "../utilities/Constants.h"
#include "../Game.h"
#include "MainMenuScreen.h"
#include "../interface/Button.h"

#include "sl.h"
#include <string>
#include <cmath>
#include <iostream>

namespace Gameplay
{
	int gameplayMusic;
	int gameplayMusicLoop;

	static int gameplayBackground;
	static int gameplayHUD;
	static int normalPalletteTexture;

	static Pallette pall;
	static Ball ball;
	static Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS];
	static bool pause;
	static bool allBricksDestroyed = false;
	static double deltaTime;
	static double collisionCooldown = 0.0;

	static const int MAX_PAUSE_BUTTONS = 3;
	static Button pauseButtons[MAX_PAUSE_BUTTONS];
	static std::string pauseButtonNames[MAX_PAUSE_BUTTONS] = { "RESUME", "RESTART", "EXIT" };

	// PRIVATE FUNCTIONS
	static bool CheckCollisionPalletteBall(Pallette pall, Ball ball);
	static bool CheckCollisionBallBrick(Ball ball, Brick& brick);
	static bool ThereBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS]);
	static bool IsPlayerAlive(Pallette pall);
	static void PrintScreenWinner();
	static void ResetBall();
	static void InitPauseMenu();
	static void DrawMenuPause();
	static void UpdateMenuPause();

	void Init()
	{
		gameplayMusic = slLoadWAV("res/music/gameplayMusic.wav");
		gameplayBackground = slLoadTexture("res/images/gameplayBackground.png");
		gameplayHUD = slLoadTexture("res/images/gameplayHUD.png");
		normalPalletteTexture = slLoadTexture("res/images/normalPallette.png");

		pall.width = 100.0;
		pall.height = 50.0;
		pall.x = SCREEN_WIDTH / 2.0;
		pall.y = pall.height;
		pall.speed = 700.0;
		pall.lives = 30;
		pall.score = 0;

		ball.radius = 15.0;
		ball.x = SCREEN_WIDTH / 2.0;
		ball.y = SCREEN_HEIGHT / 2.0;
		ball.speedX = 300.0;
		ball.speedY = 325.0;
		ball.isActive = false;

		InitBricks(bricks);
		InitBall();

		InitPauseMenu();

		pause = false;
	}

	void Input()
	{
		UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

		if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown)
		{
			pause = !pause;

			if (pause)
			{
				slSoundPause(gameplayMusicLoop);
			}
			else
			{
				slSoundResumeAll();
			}
		}

		if (!pause)
		{
			if (slGetKey('a') || slGetKey('A'))
			{
				pall.x -= pall.speed * deltaTime;
			}

			if (slGetKey('d') || slGetKey('D'))
			{
				pall.x += pall.speed * deltaTime;
			}
		}

		if (slGetKey(' '))
		{
			ball.isActive = true;
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

		if (!pause)
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
			}
			else
			{
				ball.x = pall.x;
				ball.y = pall.y + pall.height + ball.radius;

				//BALL TEST!!!
				//if (slGetKey(SL_KEY_UP))
				//{
				//	ball.y += 300 * deltaTime;
				//}
				//if (slGetKey(SL_KEY_LEFT))
				//{
				//	ball.x -= 300 * deltaTime;
				//}
				//if (slGetKey(SL_KEY_DOWN))
				//{
				//	ball.y -= 300 * deltaTime;
				//}
				//if (slGetKey(SL_KEY_RIGHT))
				//{
				//	ball.x += 300 * deltaTime;
				//}
			}

			if (ball.x - ball.radius < 0)
			{
				ball.x = ball.radius;
				ball.speedX *= -1.0;
				PlayImpactSound();
			}
			if (ball.x + ball.radius > SCREEN_WIDTH)
			{
				ball.x = SCREEN_WIDTH - ball.radius;
				ball.speedX *= -1.0;
				PlayImpactSound();
			}

			if (ball.y + ball.radius > MAXIMUM_TOP_Y)
			{
				ball.y = MAXIMUM_TOP_Y - ball.radius;
				ball.speedY *= -1.0;
				PlayImpactSound();
			}
			if (ball.y + ball.radius < 0)
			{
				ball.speedY = std::abs(ball.speedY);
				pall.lives--;
				ResetBall();
			}

			for (int row = 0; row < MAX_ROW_BRICKS; row++)
			{
				for (int col = 0; col < MAX_COL_BRICKS; col++)
				{
					if (bricks[row][col].isActive && CheckCollisionBallBrick(ball, bricks[row][col]))
					{
						double deltaX = ball.x - bricks[row][col].x;
						double deltaY = ball.y - bricks[row][col].y;

						double overlapHorizontal = (bricks[row][col].width / 2.0 + ball.radius) - fabs(deltaX);
						double overlapVertical = (bricks[row][col].height / 2.0 + ball.radius) - fabs(deltaY);

						if (overlapHorizontal < overlapVertical)
						{
							if (deltaX < 0) // LEFT
							{
								ball.x = bricks[row][col].x - bricks[row][col].width / 2.0 - ball.radius - 0.1;
								ball.speedX = -fabs(ball.speedX);

								if (ball.y > bricks[row][col].y)
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
								ball.x = bricks[row][col].x + bricks[row][col].width / 2.0 + ball.radius + 0.1;
								ball.speedX = fabs(ball.speedX);

								if (ball.y > bricks[row][col].y)
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
								ball.y = bricks[row][col].y + bricks[row][col].height / 2.0 + ball.radius + 0.1;
								ball.speedY = fabs(ball.speedY);
								
								if (ball.x < bricks[row][col].x)
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
								ball.y = bricks[row][col].y - bricks[row][col].height / 2.0 - ball.radius - 0.1;
								ball.speedY = -fabs(ball.speedY);

								if (ball.x < bricks[row][col].x)
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

						pall.score += 100;
						bricks[row][col].isActive = false;
						PlayImpactSound();
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
							std::cout << "Left TOP!" << std::endl;
						}
						else
						{
							std::cout << "Left BUTTOM!" << std::endl;
						}
					}
					else // RIGHT
					{
						ball.x = pall.x + pall.width / 2.0 + ball.radius + 0.1;
						ball.speedX = fabs(ball.speedX);

						if (ball.y > pall.y)
						{
							ball.speedY *= -1.0;
							std::cout << "Right TOP!" << std::endl;
						}
						else
						{
							std::cout << "Right BUTTOM!" << std::endl;
						}
					}
				}
				else // TOP
				{
					ball.y = pall.y + pall.height / 2.0 + ball.radius + 0.1;
					ball.speedY *= -1.0;
					std::cout << "Top!" << std::endl;
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

				PlayImpactSound();
			}
		}
		else
		{
			UpdateMenuPause();
		}
	}

	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSprite(gameplayBackground, SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);

		DrawBricks(bricks);

		slSprite(normalPalletteTexture, pall.x, pall.y, pall.width, pall.height);

		slSetForeColor(0.0, 0.0, 1.0, 1.0);
		slRectangleOutline(pall.x, pall.y, pall.width, pall.height);

		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSprite(CatBounce::normalBallTexture, ball.x, ball.y, ball.radius * 2.0, ball.radius * 2.0);

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

		if (pause)
		{
			DrawMenuPause();
		}

		if (!ThereBricks(bricks))
		{
			PrintScreenWinner();
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

	static bool CheckCollisionBallBrick(Ball ball, Brick& brick)
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

	static bool ThereBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS])
	{
		for (int row = 0; row < MAX_ROW_BRICKS; row++)
		{	
			for (int col = 0; col < MAX_COL_BRICKS; col++)
			{
				if (bricks[row][col].isActive)
				{
					return true;
				}
			}
		}

		return false;
	}

	static bool IsPlayerAlive(Pallette pall)
	{
		if (pall.lives <= 0)
		{
			return false;
		}

		return true;
	}

	static void PrintScreenWinner()
	{
		slSetForeColor(0.0, 0.0, 0.0, 0.75);
		slRectangleFill(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slText(40.0, 40.0, "You win");
	}

	static void ResetBall()
	{

		ball.isActive = false;
	}

	static void InitPauseMenu()
	{
		double btnWidth = 250.0;
		double btnHeight = 75.0;
		double marginBetween = 10.0;
		double marginBottom = 300.0;

		for (int i = 0; i < MAX_PAUSE_BUTTONS; i++)
		{
			double y = SCREEN_HEIGHT - marginBottom - (btnHeight + marginBetween) * (i + 1);

			pauseButtons[i] = CreateButton((SCREEN_WIDTH / 2.0) - (btnWidth / 2.0), y, btnWidth, btnHeight, pauseButtonNames[i]);
		}
	}

	static void DrawMenuPause()
	{
		slSetForeColor(0.0, 0.0, 0.0, 0.8);
		slRectangleFill(SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0, SCREEN_WIDTH, SCREEN_HEIGHT);

		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slSetFont(CatBounce::specialFont, 50);
		double textWidth = slGetTextWidth("PAUSED");
		slText(SCREEN_WIDTH / 2.0 - textWidth / 2.0, SCREEN_HEIGHT / 2.0 + 180, "PAUSED");

		for (int i = 0; i < MAX_PAUSE_BUTTONS; i++)
		{
			DrawButton(pauseButtons[i]);
		}
	}

	static void UpdateMenuPause()
	{
		for (int i = 0; i < MAX_PAUSE_BUTTONS; i++)
		{
			UpdateButton(pauseButtons[i]);

			if (pauseButtons[i].clicked)
			{
				if (pauseButtonNames[i] == "RESUME")
				{
					slSoundPlay(CatBounce::buttonPressed);
					pause = false;
					slSoundResumeAll();
				}
				else if (pauseButtonNames[i] == "RESTART")
				{
					slSoundPlay(CatBounce::buttonPressed);
					InitBricks(bricks);
					pall.lives = 3;
					pall.score = 0;
					pause = false;
					slSoundResumeAll();
				}
				else if (pauseButtonNames[i] == "EXIT")
				{
					slSoundPlay(CatBounce::buttonPressed);
					slSoundStop(gameplayMusicLoop);
					MainMenu::mainMenuMusicLoop = slSoundLoop(MainMenu::mainMenuMusic);
					CatBounce::currentScene = CatBounce::Scenes::MainMenu;
				}
			}
		}
	}
}