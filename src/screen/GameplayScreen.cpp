#include "GameplayScreen.h"
#include "../entities/Pallette.h"
#include "../entities/Ball.h"
#include "../entities/Brick.h"
#include "../utilities/Constants.h"
#include "../Game.h"
#include "MainMenuScreen.h"

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

	// PRIVATE FUNCTIONS
	static bool CheckCollisionPalletteBall(Pallette pall, Ball ball);
	static bool CheckCollisionBallBrick(Ball ball, Brick& brick);
	static bool ThereBricks(Brick bricks[MAX_ROW_BRICKS][MAX_COL_BRICKS]);
	static bool IsPlayerAlive(Pallette pall);
	static void PrintScreenWinner();
	static void ResetBall();

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
		pall.speed = 600.0;
		pall.speed = 600.0;
		pall.lives = 30;
		pall.score = 0;

		ball.radius = 15.0;
		ball.x = SCREEN_WIDTH / 2.0;
		ball.y = SCREEN_HEIGHT / 2.0;
		ball.speedX = 300.0;
		ball.speedY = 375.0;
		ball.isActive = false;

		InitBricks(bricks);

		pause = false;
	}

	void Input()
	{
		if (!pause)
		{
			UpdateKey(CatBounce::inputSystem, SL_KEY_ESCAPE);

			if (GetKeyState(CatBounce::inputSystem) == KeyState::KeyDown)
			{
				slSoundStop(gameplayMusicLoop);
				MainMenu::mainMenuMusicLoop = slSoundLoop(MainMenu::mainMenuMusic);
				CatBounce::currentScene = CatBounce::Scenes::MainMenu;
			}

			if (slGetKey('a') || slGetKey('A'))
			{
				pall.x -= pall.speed * deltaTime;
			}

			if (slGetKey('d') || slGetKey('D'))
			{
				pall.x += pall.speed * deltaTime;
			}
		}

		// TEST!!!
		if (slGetKey(' '))
		{
			pause = true;
		}
		else
		{
			pause = false;
		}

		if (slGetKey('E'))
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
			}
			if (ball.x + ball.radius > SCREEN_WIDTH)
			{
				ball.x = SCREEN_WIDTH - ball.radius;
				ball.speedX *= -1.0;
			}

			if (ball.y + ball.radius > MAXIMUM_TOP_Y)
			{
				ball.y = MAXIMUM_TOP_Y - ball.radius;
				ball.speedY *= -1.0;
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
						pall.score += 100;
						bricks[row][col].isActive = false;
						ball.speedY *= -1.0;
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

				collisionCooldown = 0.5;
			}

			if (!IsPlayerAlive(pall))
			{
				std::cout << "Loser!!!";
			}

			if (!ThereBricks(bricks))
			{
				std::cout << "Winner!!!";
			}
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
}