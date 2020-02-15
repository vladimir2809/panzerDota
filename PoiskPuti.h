#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iostream>
using namespace std;
class PoiskPuti{
	Stens stens;
	struct Point{
		int x, y;
	};
	struct PointAim{
		int x, y;
		bool being;
	};
	struct pointSearch{
		int num;
		int x, y;
		float H, F, G=0;
		bool visited=false;
		Point parent;
	};
	int xPanzer, yPanzer;
	vector<Point> barier;//������ � ������� �������� �����������
	vector<Point> barierMove;// ������ �������������� �����������
	vector<PointAim> pointsAim;//������ ����� ���� ����
	Point finish,start;// ����� ������ � ������
	vector<pointSearch> open;// ������ � ��������� ��������
	vector<pointSearch> close;// ������ � ��������� ��������, ������ �����������
	vector<PointRoute> route;// �������
	bool beingRoute;// ������ �� �������
	float addG;// ��������� ��� ������� ���������� ������������ ��������� ��� ������ ����
public:
	PoiskPuti(Stens &stens){
		addG = 7;
		beingRoute = false;
		// ������� ����� � ������ � ���������
		for (int i = 0; i < kolvoStens; i++)
		{
			Point oneBarier;
			oneBarier.x = (stens.stena[i].x)/ stens.size;
			oneBarier.y = (stens.stena[i].y) / stens.size;
			barier.push_back(oneBarier);
		}
	}

	void updateXYPanzer(int x, int y)
	{
		xPanzer = x; 
		yPanzer = y;
	}
	void updateBarierMove()// �������� ������ �������������� ����������
	{
		barierMove.clear();
		for (int i = 0; i < panzersData.size(); i++)
		{
		//	if (start.x / stens.size != (panzersData[i].x) / stens.size
		//		|| start.y / stens.size != (panzersData[i].y) / stens.size)
			{
				Point oneBarier;
				if ((xPanzer / stens.size == (panzersData[i].x) / stens.size
					&& yPanzer / stens.size == (panzersData[i].y) / stens.size) == false)
				{
					oneBarier.x = (panzersData[i].x) / stens.size;
					oneBarier.y = (panzersData[i].y) / stens.size;
					barierMove.push_back(oneBarier);
				}
				if (((xPanzer + panzersData[i].sizeX) / stens.size == (panzersData[i].x + panzersData[i].sizeX) / stens.size
					&& yPanzer / stens.size == (panzersData[i].y) / stens.size) == false)
				{
					oneBarier.x = (panzersData[i].x + panzersData[i].sizeX) / stens.size;
					oneBarier.y = (panzersData[i].y) / stens.size;
					barierMove.push_back(oneBarier);
				}
				if ((xPanzer / stens.size == (panzersData[i].x) / stens.size
					&& (yPanzer + panzersData[i].sizeY) / stens.size == (panzersData[i].y + panzersData[i].sizeY) / stens.size) == false)
				{
					oneBarier.x = (panzersData[i].x) / stens.size;
					oneBarier.y = (panzersData[i].y + panzersData[i].sizeY) / stens.size;
					barierMove.push_back(oneBarier);
				}
				if (((xPanzer + panzersData[i].sizeX) / stens.size == (panzersData[i].x + panzersData[i].sizeX) / stens.size
					&& (yPanzer + panzersData[i].sizeY) / stens.size == (panzersData[i].y + panzersData[i].sizeY) / stens.size) == false)
				{
					oneBarier.x = (panzersData[i].x + panzersData[i].sizeX) / stens.size;
					oneBarier.y = (panzersData[i].y + panzersData[i].sizeY) / stens.size;
					barierMove.push_back(oneBarier);
				}
			}
		}
	}

	void DrawRoute(RenderWindow &window, Text text)// ���������� ������� 
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));
		for (int i = 0; i < route.size(); i++)
		{
			{
				rectangle.setFillColor(Color(0, 255, 0));
				rectangle.setPosition(route[i].x*stens.size + 15, route[i].y*stens.size + 15);
				window.draw(rectangle);
				//������� ���������
				DrawNumTxt(route[i].x*stens.size, route[i].y*stens.size + 8, i, window, text);
			}
		}
	}
	
	void DrawPoisk(RenderWindow &window,Text text)// �������� �������� � �������� ������ ����� ��� �������
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));

		for (int i = 0; i < open.size(); i++)// ������� �������� ������
		{
			{
				rectangle.setFillColor(Color(255, 0, 0));
				rectangle.setPosition(open[i].x*stens.size + 15, open[i].y*stens.size + 15);
				window.draw(rectangle);
				DrawNumTxt(open[i].x*stens.size, open[i].y*stens.size + 8, open[i].F, window, text);
			}
		}
		for (int i = 0; i < close.size(); i++)//������� �������� ������
		{
			{
				rectangle.setFillColor(Color(0, 0, 255));
				rectangle.setPosition(close[i].x*stens.size + 15, close[i].y*stens.size + 15);
				window.draw(rectangle);
				DrawNumTxt(close[i].x*stens.size, close[i].y*stens.size + 8, close[i].F, window, text);

				//DrawNumTxt(close[i].x*stens.size, close[i].y*stens.size + 18, close[i].G, window, text);
				//DrawNumTxt(close[i].x*stens.size + 20, close[i].y*stens.size + 18, close[i].H, window, text);

				//DrawNumTxt(close[i].x*stens.size, close[i].y*stens.size + 28, close[i].x, window, text);
				//DrawNumTxt(close[i].x*stens.size + 20, close[i].y*stens.size + 28, close[i].y, window, text);

				//DrawNumTxt(close[i].x*stens.size, close[i].y*stens.size + 38, close[i].parent.x, window, text);
				//DrawNumTxt(close[i].x*stens.size + 20, close[i].y*stens.size + 38, close[i].parent.y, window, text);
			}
		}
		for (int i = 0; i < barier.size(); i++)// ������� �����������
		{
			{
				rectangle.setFillColor(Color(255, 255, 0));
				rectangle.setPosition(barier[i].x*stens.size + 15, barier[i].y*stens.size + 15);
				window.draw(rectangle);
			}
		}
		for (int i = 0; i < barierMove.size(); i++)// ������� ����������� move
		{
			{
				rectangle.setFillColor(Color(155, 125, 225));
				rectangle.setPosition(barierMove[i].x*stens.size + 15, barierMove[i].y*stens.size + 15);
				window.draw(rectangle);
			}
		}
		for (int i = 0; i < pointsAim.size(); i++)// ������� ����������� move
		{
			{
				rectangle.setFillColor(Color(0, 125, 225));
				rectangle.setPosition(pointsAim[i].x*stens.size + 15, pointsAim[i].y*stens.size + 15);
				DrawNumTxt(pointsAim[i].x*stens.size, pointsAim[i].y*stens.size + 10, i, window, text);
				window.draw(rectangle);
			}
		}
		DrawRoute(window, text);// ������� �������
		//������� ����� ������
		rectangle.setFillColor(Color(0, 255, 255));
		rectangle.setPosition(start.x*stens.size + 15, start.y*stens.size + 15);
		window.draw(rectangle);
		//		if (MapPoisk[i][j]>0)
				{
					//text.setColor(Color(255,255,255));//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
					//std::ostringstream numRastStr;    // �������� ����������
					//numRastStr <<MapPoisk[i][j];
					//text.setString(numRastStr.str());
					//   text.setPosition(i*40+20,j*40+20);
					//window.draw(text);
					// rectangle.setFillColor(Color(108+MapPoisk[i][j]*8, 128, 0));

					// window.draw(rectangle);
				}


	}

	float Raschet_H(pointSearch point, Point finishPoint)// ������� ������� ������������������ ��������� �� ������
	{
		float dx, dy;
		dx = finishPoint.x - point.x;
		dy = finishPoint.y - point.y;
		float res;
		res = sqrt(dx*dx + dy*dy);
		return res*10;
	}
	bool IsBarier(int x,int y)// ���� � � ��� �����������
	{
		for (int i = 0; i < barier.size(); i++)
		{
			if (x == barier[i].x && y == barier[i].y) return  true;
		}
		for (int i = 0; i < barierMove.size(); i++)
		{
			if (x == barierMove[i].x && y == barierMove[i].y) return  true;
		}
		return false;
	}
	bool IsClose(int x, int y)// ���� � � ��� �������� ������ ��������� � �������� ������
	{
		for (int i = 0; i < close.size(); i++)
		{
			if (x == close[i].x && y == close[i].y) return  true;
		}
		return false;
	}
	bool IsOpen(int x, int y)// ���� � � ��� ������ ��������� � �������� ������
	{
		for (int i = 0; i < open.size(); i++)
		{
			if (x == open[i].x && y == open[i].y) return  true;
		}
		return false;
	}
	int NumIsOpen(int x,int y)// ����� ������ � ������� ��������� ������ �� � � 
	{
		for (int i = 0; i < open.size(); i++)
		{
			if (x == open[i].x && y == open[i].y) return  i;
		}
		return -1;
	}
	int NumIsClose(int x, int y)// ����� ������ � ������� ��������� ������
	{
		for (int i = 0; i < close.size(); i++)
		{
			if (x == close [i].x && y == close[i].y) return  i;
		}
		return -1;
	}
	bool IsPointAim(int x, int y)// ���� � � ��� ������ ��� ����� ���� ������
	{
		for (int i = 0; i < pointsAim.size(); i++)
		{
			if (x == pointsAim[i].x && y == pointsAim[i].y) return  true;
		}
		return false;
	}
	void RashetPointAim(int aimX, int aimY, int countPoint)// ��������� ����� ���� ��� �������� ��������� ������ ������
	{
		PointAim actual;
		pointsAim.clear();
		actual.x = aimX;
		actual.y = aimY;
		pointsAim.push_back(actual);
		//
		int i = 0;
		do
		{
			
				if (IsBarier(pointsAim[i].x - 1, pointsAim[i].y) == false
					&& IsPointAim(pointsAim[i].x - 1, pointsAim[i].y) == false && pointsAim[i].x - 1>=0)
				{
					actual.x = pointsAim[i].x - 1;
					actual.y = pointsAim[i].y;
					pointsAim.push_back(actual);
					if (pointsAim.size() == countPoint) break;
					//	continue;
				}
				if (IsBarier(pointsAim[i].x + 1, pointsAim[i].y) == false
					&& IsPointAim(pointsAim[i].x + 1, pointsAim[i].y) == false && pointsAim[i].x + 1 <= map_Size_X)
				{
					actual.x = pointsAim[i].x + 1;
					actual.y = pointsAim[i].y;
					pointsAim.push_back(actual);
					if (pointsAim.size() == countPoint ) break;
					//continue;
				}
				if (IsBarier(pointsAim[i].x, pointsAim[i].y - 1) == false
					&& IsPointAim(pointsAim[i].x, pointsAim[i].y - 1) == false && pointsAim[i].y - 1 >= 0)
				{
					actual.x = pointsAim[i].x;
					actual.y = pointsAim[i].y - 1;
					pointsAim.push_back(actual);
					if (pointsAim.size() == countPoint ) break;
					//	continue;
				}
				if (IsBarier(pointsAim[i].x, pointsAim[i].y + 1) == false
					&& IsPointAim(pointsAim[i].x, pointsAim[i].y + 1) == false && pointsAim[i].y + 1 <= map_Size_Y)
				{
					actual.x = pointsAim[i].x;
					actual.y = pointsAim[i].y + 1;
					pointsAim.push_back(actual);
					if (pointsAim.size() == countPoint ) break;
					//	continue;
				}
				i++;
				
		} while (pointsAim.size() < countPoint);

	}
	void RaschetInStep(int dx,int dy,pointSearch actual)// ������� ������� ���� ��� ���������� ����� ������ ����
	{
		pointSearch point;
		// �������� point ��������������� ������ 
		point.x = actual.x+dx;
		point.y = actual.y +dy;
		if (IsOpen(point.x, point.y) == false)// ���� ����� �� � �������� ������, ������ �� ��� �� �������������
		{
			// ��������� F G H 
			point.H = Raschet_H(point, finish);
			point.G = actual.G + addG;
			point.F = point.G + point.H;
			// �������� ��������
			point.parent.x = actual.x;
			point.parent.y = actual.y;
			open.push_back(point);//������� ����� � �������� ������
		}
		else// ����� ���� ����� ��� ����  � �������� ������
		{
			int num = NumIsOpen(point.x,point.y);// num �������� ����� ������ � �������� ������ ������� �������������
			if (actual.G + addG <= open[num].G)// ���� ������ ����������� ��������� ������ � �������� ������
				// ������ ��� �� ������� ����� ���� �� ������ �� �������� ������ ��� �������
			{
				// ��������� ����� F G H 
				open[num].G = actual.G + addG;
				open[num].H = Raschet_H(open[num], finish);
				open[num].F = open[num].G + open[num].H;
				// �������� ������ ��������
				open[num].parent.x = actual.x;
				open[num].parent.y = actual.y;
			}
		}
	}
	// ������� ��������������� ����� ������ ����
	void VolnaPuti(int startX, int startY, int arrivalX, int arrivalY,RenderWindow &window, Text &text)
	{
		// ���������� ������ ������ �� ����������� ������
		open.clear();
		close.clear();
		beingRoute = false;
		RectangleShape rectangle(sf::Vector2f(10, 10));
		pointSearch	point, 
					actual;// ������ ������� ������ ������������ �� ��� ������� �� ��������
		
		point.x = startX / stens.size;// ����������� ����� ����� ������
		point.y = startY / stens.size;
		start.x = point.x;// �������������� �����
		start.y = point.y;
		finish.x = arrivalX / stens.size;// ������������� �����
		finish.y = arrivalY / stens.size;
		// ��������� ��� ����� ������ F G H
		point.G = 0;
		point.H = Raschet_H(point, finish);
		point.F = 0+Raschet_H(point,finish);
		// �������� ����� ������ � � �������� -1
		point.parent.x = -1;
		point.parent.y = -1;
		open.push_back(point);// ������� ������ ����� � �������� ������
		if (!(finish.x == start.x && finish.y == start.y))
		do
		{
			float minF = 100000000;
			int numI = 0;
			for (int i = 0; i < open.size(); i++)// ������ ���������� ����� � ���������� ������� ���������
			{
				if (open[i].F < minF)
				{
					actual = open[i];
					minF = open[i].F;
					numI = i;
				}
			}
			// ������� �� ���������� �������� � ��������
			close.push_back(actual);
			open.erase(open.begin()+ numI);
			// �������� �������� ������
			if (actual.y - 1>=0)
				if (IsBarier(actual.x, actual.y - 1) == false && IsClose(actual.x, actual.y - 1) == false)
				{
					RaschetInStep(0, -1, actual);
				}
			if (actual.x + 1 < map_Size_X)
				if (IsBarier(actual.x + 1, actual.y) == false && IsClose(actual.x + 1, actual.y) == false)
				{
					RaschetInStep(1, 0, actual);
				}
			if (actual.y + 1 <= map_Size_Y)
				if (IsBarier(actual.x, actual.y + 1) == false && IsClose(actual.x, actual.y + 1) == false)
				{
					RaschetInStep(0, 1, actual);
				}
			if (actual.x - 1 >= 0)
				if (IsBarier(actual.x - 1, actual.y) == false && IsClose(actual.x - 1, actual.y) == false)
				{
					RaschetInStep(-1, 0, actual);
				}
			if (Keyboard::isKeyPressed(Keyboard::Y))
			{
				Sleep(10);
			}
			//window.clear();
			//DrawPoisk(window,text);

			//rectangle.setFillColor(Color(255, 255,255));
			//rectangle.setPosition(actual.x*stens.size + 15, actual.y*stens.size + 15);
			//window.draw(rectangle);
			//if (Keyboard::isKeyPressed(Keyboard::T))
			//{
			//	for (int i = 0; i < open.size(); i++)
			//	{
			//		text.setColor(Color(255, 255, 255));//��������� ����� � �������. ���� ������ ��� ������, �� �� ��������� �� �����
			//		std::ostringstream numRastStr;    // �������� ����������
			//		numRastStr << trunc(open[i].F);
			//		text.setString(numRastStr.str());
			//		text.setPosition(open[i].x*stens.size, open[i].y*stens.size + 8);
			//		window.draw(text);
			//	}
			//}
			//window.display();
			//Sleep(2);
			if (actual.x == finish.x && actual.y == finish.y)// ���� ���������� ����� ����� �� ����� ������
			{
				beingRoute = true;// �������� ������ ���� ��� ������� ������
			}
		} while (open.size()>0 && !beingRoute);// ������� �� ����� ���� ���������� ��� ��������� ������ ��� ������� ������
	}
	//void InStepCreateRoute(pointSearch &pointRoute, int it)
	//{
	//	PointRoute rouPo;
	//	rouPo.x = pointRoute.x;
	//	rouPo.y = pointRoute.y;
	//	route.push_back(rouPo);
	//	pointRoute.x = close[it].parent.x;
	//	pointRoute.y = close[it].parent.y;
	//}
	void InStepCreateRoute(pointSearch &pointRoute, int it)// ������� ������� ���� ��� ���������� ��������
	{
		PointRoute rouPo;
		rouPo.x = pointRoute.x;
		rouPo.y = pointRoute.y;
		route.push_back(rouPo);// ������� ����� � ������� 
		pointRoute.x = close[it].parent.x;// ������� ����� � � ������� ������������ � �
		pointRoute.y = close[it].parent.y;
	}
	vector<PointRoute> CreateRoute(RenderWindow &window, Text &text)
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));
		pointSearch pointRoute;// ������� ����� ����
		PointRoute rouPo;
		route.clear();//������� �������
		if (beingRoute)// ���� ������� ������ 
		{
			// ����� �������� ����� ������
			int numFinish = NumIsClose(finish.x,finish.y);
			int numPoint = numFinish;
			pointRoute = close[numFinish];
			do
			{
				for (int i = 0; i < close.size(); i++)//���� �� ���� ������ ��������� ������
				{					
					if (pointRoute.x == close[i].x && pointRoute.y == close[i].y )// ���� �������� ������� ����� (�����)
						// ����� � � ����� ��������� ������
					{
						InStepCreateRoute(pointRoute, i);// ����������� ���.
						break;
					}
					
				}
				//������� ����� ������ � �������
				if ((pointRoute.x == start.x && pointRoute.y == start.y))
				{
					rouPo.x = pointRoute.x;
					rouPo.y = pointRoute.y;
					route.push_back(rouPo); 
				}
			/*	window.clear();
				DrawPoisk(window,text);
				for (int i = 0; i < route.size(); i++)
				{
					rectangle.setFillColor(Color(255, 255, 255));
					rectangle.setPosition(route[i].x*stens.size + 15, route[i].y*stens.size + 15);
					window.draw(rectangle);
				}
				window.display();
				Sleep(2);*/
			} while ((pointRoute.x != start.x || pointRoute.y != start.y));// ��������� ���� ���� ����� �� ����� ������
		}
		reverse (route.begin(),route.end() );// ����������� ������� ��� ��� ��� ������� � �����
		return route;// ��������� �������
	}
	
};
// ���� ������� ��� ��� ���� ����� ���������� �� ���������� �� �� ����������� ����� ����