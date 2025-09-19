#include "GameplayScreen.h"
#include "../entities/Pallette.h"
#include "../entities/Ball.h"
#include "../entities/Brick.h"
#include "../utilities/Constants.h"
#include "../Game.h"
#include "sl.h"

#include <string>
#include <cmath>
#include <iostream>

namespace Gameplay
{
	static Pallette pall;
	static Ball ball;
	static Brick bricks[MAX_BRICKS][MAX_BRICKS];
	static bool pause;
	static bool allBricksDestroyed = false;
	static double deltaTime;

	// PRIVATE FUNCTIONS
	static bool CheckCollisionPalletteBall(Pallette pall, Ball ball);
	static bool CheckCollisionBallBrick(Ball ball, Brick& brick);
	static bool ThereBricks(Brick bricks[MAX_BRICKS][MAX_BRICKS]);
	static bool IsPlayerAlive(Pallette pall);
	static void PrintScreenWinner();
	static void ResetBall();

	void Init()
	{
		pall.width = 100.0;
		pall.height = 30.0;
		pall.x = SCREEN_WIDTH / 2.0;
		pall.y = pall.height;
		pall.speed = 600.0;
		pall.lives = 3;

		ball.radius = 15.0;
		ball.x = SCREEN_WIDTH / 2.0;
		ball.y = SCREEN_HEIGHT / 2.0;
		ball.speedX = 300.0;
		ball.speedY = 400.0;
		ball.isActive = false;

		InitBricks(bricks);

		pause = false;
	}

	void Input()
	{
		if (!pause)
		{
			if (slGetKey(SL_KEY_ESCAPE))
			{
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

			if (ball.y + ball.radius > SCREEN_HEIGHT)
			{
				ball.y = SCREEN_HEIGHT - ball.radius;
				ball.speedY *= -1.0;
			}
			if (ball.y + ball.radius < 0)
			{
				ball.speedY = std::abs(ball.speedY);
				pall.lives--;
				ResetBall();
			}

			for (int row = 0; row < MAX_BRICKS; row++)
			{
				for (int col = 0; col < MAX_BRICKS; col++)
				{
					if (bricks[row][col].isActive && CheckCollisionBallBrick(ball, bricks[row][col]))
					{
						bricks[row][col].isActive = false;
						ball.speedY *= -1.0;
					}
				}
			}

			if (CheckCollisionPalletteBall(pall, ball))
			{
				if (ball.y - ball.radius < pall.y + pall.height / 2.0 &&
					ball.y - ball.radius > pall.y - pall.height / 2.0 &&
					ball.x > pall.x - pall.width / 2.0 - ball.radius &&
					ball.x < pall.x + pall.width / 2.0 + ball.radius)
				{
					ball.y = pall.y + pall.height / 2.0 + ball.radius;
					std::cout << "Superior!" << std::endl;
				}
				else
				{
					if (ball.x < pall.x)
					{
						ball.x = pall.x - pall.width / 2.0 - ball.radius;
						ball.speedX = -fabs(ball.speedX);
						std::cout << "Left!" << std::endl;
					}
					else
					{
						ball.x = pall.x + pall.width / 2.0 + ball.radius;
						ball.speedX = fabs(ball.speedX);
						std::cout << "Right!" << std::endl;
					}
				}

				ball.speedY *= -1.0;

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

		DrawBricks(bricks);

		slRectangleFill(pall.x, pall.y, pall.width, pall.height);

		slSetForeColor(0.0, 0.0, 1.0, 1.0);
		slRectangleOutline(pall.x, pall.y, pall.width, pall.height);


		slSetForeColor(1.0, 1.0, 1.0, 1.0);
		slCircleFill(ball.x, ball.y, ball.radius, 200);

		slSetForeColor(1.0, 0.0, 0.0, 1.0);
		slRectangleOutline(ball.x, ball.y, ball.radius * 2.0, ball.radius * 2.0);

		std::string textLive = "Live " + std::to_string(pall.lives);
		slText(300, 100, textLive.c_str());

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

	static bool ThereBricks(Brick bricks[MAX_BRICKS][MAX_BRICKS])
	{
		for (int row = 0; row < MAX_BRICKS; row++)
		{	
			for (int col = 0; col < MAX_BRICKS; col++)
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