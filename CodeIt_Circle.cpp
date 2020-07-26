// ConsoleApplication4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<thread>
#include <string>
using namespace std;

#include <chrono> 
using namespace std::chrono;



#include "olcConsoleGameEngine.h"


struct sBall
{
	float px, py, vx, vy, ax, ay;
	float radius;

	int id;
};

struct sCircle
{
	float px, py, radius;
	int id;
};
class CirleGame : public olcConsoleGameEngine
{
public:
	CirleGame()
	{
	
		m_sAppName = L"Circle Physics";
	}
	virtual bool OnUserCreate()
	{
	
		//Unit circle
		totalNumber = 7;
		for (int i = 0; i < totalNumber; i++)
		{
			int xRand = 10 + rand() % (ScreenWidth() - 20);
			int yRand = 10 + rand() % (ScreenHeight() - 20);
			int rRand = 2 + rand() % 4;
			sCircle temp;
			temp.px = xRand;
			temp.py = yRand;
			temp.radius = rRand;
			temp.id = i;
			vecBalls.push_back(temp);

		}


		return true;

	}
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		
		auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
		{
			return fabs((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2)) <= (r1 + r2)*(r1 + r2);
			//square root is very costly here;
		};

		auto IsPointInCircle = [](float x1, float y1, float r1, float px, float py)
		{
			return fabs((x1 - px)*(x1 - px) + (y1 - py)*(y1 - py)) < (r1 * r1);
		};


		if (m_mouse[0].bPressed || m_mouse[1].bPressed)
		{
			pSelectedBall = nullptr;
			for (auto &ball : vecBalls)
			{
				if (IsPointInCircle(ball.px, ball.py, ball.radius, m_mousePosX, m_mousePosY))
				{
					pSelectedBall = &ball;
					break;
				}
			}
		}

		if (m_mouse[0].bHeld)
		{
			if (pSelectedBall != nullptr)
			{
				pSelectedBall->px = m_mousePosX;
				pSelectedBall->py = m_mousePosY;
			}
		}

		if (m_mouse[0].bReleased)
		{
			pSelectedBall = nullptr;
		}

		// Static collisions, i.e. overlap
		for (auto &ball : vecBalls)
		{
			for (auto &target : vecBalls)
			{
				if (ball.id != target.id)
				{
					if (DoCirclesOverlap(ball.px, ball.py, ball.radius, target.px, target.py, target.radius))
					{
						// Collision has occured
						//*****************vecCollidingPairs.push_back({ &ball, &target });

						// Distance between ball centers
						float fDistance = sqrtf((ball.px - target.px)*(ball.px - target.px) + (ball.py - target.py)*(ball.py - target.py));

						// Calculate displacement required
						float fOverlap = 0.5f * (fDistance - ball.radius - target.radius);

						// Displace Current Ball away from collision
						ball.px -= fOverlap * (ball.px - target.px) / fDistance;
						ball.py -= fOverlap * (ball.py - target.py) / fDistance;

						// Displace Target Ball away from collision
						target.px += fOverlap * (ball.px - target.px) / fDistance;
						target.py += fOverlap * (ball.py - target.py) / fDistance;
					}
				}
			}
		}

		Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
		for (auto s : vecBalls) DrawCircle(s.px, s.py, s.radius);
		return true;

	}

private:
	vector<sCircle> vecBalls;
	sCircle *pSelectedBall = nullptr;
	int totalNumber;
	//it is a vector point in the circle for drawing purpose
	
};



int main()
{


	CirleGame game;
	game.ConstructConsole(80, 48, 16, 16);
	// width, height , charcter sizes

	game.Start();




	return 0;
}



