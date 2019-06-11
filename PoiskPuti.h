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
	struct PointPoisk{
		int num;
		int x, y;
		float H, F, G=0;
		bool visited=false;
		Point parent;
	};
	vector<Point> barier;//������ � ������� �������� �����������
	Point finish,start;// ����� ������ � ������
	vector<PointPoisk> open;// ������ � ��������� ��������
	vector<PointPoisk> close;// ������ � ��������� ��������, ������ �����������
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

	float Raschet_H(PointPoisk point, Point finishPoint)// ������� ������� ������������������ ��������� �� ������
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
	void RaschetInStep(int dx,int dy,PointPoisk actual)// ������� ������� ���� ��� ���������� ����� ������ ����
	{
		PointPoisk point;
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
		PointPoisk	point, 
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
	//void InStepCreateRoute(PointPoisk &pointRoute, int it)
	//{
	//	PointRoute rouPo;
	//	rouPo.x = pointRoute.x;
	//	rouPo.y = pointRoute.y;
	//	route.push_back(rouPo);
	//	pointRoute.x = close[it].parent.x;
	//	pointRoute.y = close[it].parent.y;
	//}
	void InStepCreateRoute(PointPoisk &pointRoute, int it)// ������� ������� ���� ��� ���������� ��������
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
		PointPoisk pointRoute;// ������� ����� ����
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