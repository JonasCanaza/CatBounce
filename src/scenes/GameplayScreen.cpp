#include "GameplayScreen.h"
#include "../entities/Pallette.h"
#include "../entities/Ball.h"
#include "../utilities/Constants.h"
#include "sl.h"

namespace Gameplay
{
	Pallette pall;
	Ball ball;
	double deltaTime = slGetDeltaTime();

	void Init()
	{
		pall.width = 100.0;
		pall.height = 30.0;
		pall.x = WIDTH_SCREEN / 2.0;
		pall.y = pall.height;
		pall.speed = 600.0;

		ball.radius = 15.0;
		ball.x = WIDTH_SCREEN / 2.0;
		ball.y = HEIGHT_SCREEN / 2.0;
		ball.speedX = 200.0;
		ball.speedY = 250.0;
	}

	void Input()
	{
		if (slGetKey('a') || slGetKey('A'))
		{
			pall.x -= pall.speed * deltaTime;
		}
		if (slGetKey('d') || slGetKey('D'))
		{
			pall.x += pall.speed * deltaTime;
		}

		// TEST!!!
		if (slGetKey(' '))
		{
			ball.y += ball.speedY * deltaTime;
		}
	}

	void Update()
	{
		if (pall.x - pall.width / 2.0 < 0)
		{
			pall.x = pall.width / 2.0;
		}

		if (pall.x + pall.width / 2.0 > WIDTH_SCREEN)
		{
			pall.x = WIDTH_SCREEN - pall.width / 2;
		}

		if (ball.y + ball.radius > HEIGHT_SCREEN)
		{
			ball.speedY *= -1.0;
		}

		if (ball.y - ball.radius < 0)
		{
			ball.y = HEIGHT_SCREEN / 2.0;
			ball.speedY *= -1.0;
		}
	}

	void Draw()
	{
		slSetBackColor(0.0, 0.0, 0.0);
		slSetForeColor(1.0, 1.0, 1.0, 1.0);

		slRectangleFill(pall.x, pall.y, pall.width, pall.height);
		slRectangleOutline(pall.x, pall.y, pall.width, pall.height);

		slCircleFill(ball.x, ball.y, ball.radius, 200);

		slRender();
	}
}