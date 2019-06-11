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
	vector<Point> barier;//маасив в котором хранятся препятствия
	Point finish,start;// точки старта и финиша
	vector<PointPoisk> open;// список с открытыми клетками
	vector<PointPoisk> close;// список с закрытыми клетками, тоесть посешенными
	vector<PointRoute> route;// маршрут
	bool beingRoute;// найден ли маршрут
	float addG;// добавлеия при расчете фактически затраченного растояния при поиске пути
public:
	PoiskPuti(Stens &stens){
		addG = 7;
		beingRoute = false;
		// заносим стены в миссив с барьерами
		for (int i = 0; i < kolvoStens; i++)
		{
			Point oneBarier;
			oneBarier.x = (stens.stena[i].x)/ stens.size;
			oneBarier.y = (stens.stena[i].y) / stens.size;
			barier.push_back(oneBarier);
		}
	}
	
	void DrawRoute(RenderWindow &window, Text text)// нарисовать маршрут 
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));
		for (int i = 0; i < route.size(); i++)
		{
			{
				rectangle.setFillColor(Color(0, 255, 0));
				rectangle.setPosition(route[i].x*stens.size + 15, route[i].y*stens.size + 15);
				window.draw(rectangle);
				//выведем растояние
				DrawNumTxt(route[i].x*stens.size, route[i].y*stens.size + 8, i, window, text);
			}
		}
	}
	
	void DrawPoisk(RenderWindow &window,Text text)// нарисыем открытый и закрытый список нужно для отладки
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));

		for (int i = 0; i < open.size(); i++)// выводим открытый список
		{
			{
				rectangle.setFillColor(Color(255, 0, 0));
				rectangle.setPosition(open[i].x*stens.size + 15, open[i].y*stens.size + 15);
				window.draw(rectangle);
				DrawNumTxt(open[i].x*stens.size, open[i].y*stens.size + 8, open[i].F, window, text);
			}
		}
		for (int i = 0; i < close.size(); i++)//выводим закрытый список
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
		for (int i = 0; i < barier.size(); i++)// выводим пряпитствия
		{
			{
				rectangle.setFillColor(Color(255, 255, 0));
				rectangle.setPosition(barier[i].x*stens.size + 15, barier[i].y*stens.size + 15);
				window.draw(rectangle);
			}
		}
		DrawRoute(window, text);// выведем маршрут
		//выведем точку старта
		rectangle.setFillColor(Color(0, 255, 255));
		rectangle.setPosition(start.x*stens.size + 15, start.y*stens.size + 15);
		window.draw(rectangle);
		//		if (MapPoisk[i][j]>0)
				{
					//text.setColor(Color(255,255,255));//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
					//std::ostringstream numRastStr;    // объявили переменную
					//numRastStr <<MapPoisk[i][j];
					//text.setString(numRastStr.str());
					//   text.setPosition(i*40+20,j*40+20);
					//window.draw(text);
					// rectangle.setFillColor(Color(108+MapPoisk[i][j]*8, 128, 0));

					// window.draw(rectangle);
				}


	}

	float Raschet_H(PointPoisk point, Point finishPoint)// функция расчета предположительного растояния до финиша
	{
		float dx, dy;
		dx = finishPoint.x - point.x;
		dy = finishPoint.y - point.y;
		float res;
		res = sqrt(dx*dx + dy*dy);
		return res*10;
	}
	bool IsBarier(int x,int y)// если х у это препятствие
	{
		for (int i = 0; i < barier.size(); i++)
		{
			if (x == barier[i].x && y == barier[i].y) return  true;
		}
		return false;
	}
	bool IsClose(int x, int y)// если х у это закрытая клетка находится в закрытом списке
	{
		for (int i = 0; i < close.size(); i++)
		{
			if (x == close[i].x && y == close[i].y) return  true;
		}
		return false;
	}
	bool IsOpen(int x, int y)// если х у это клетка находится в открытом списке
	{
		for (int i = 0; i < open.size(); i++)
		{
			if (x == open[i].x && y == open[i].y) return  true;
		}
		return false;
	}
	int NumIsOpen(int x,int y)// номер клетки в массиве открытого списка по х у 
	{
		for (int i = 0; i < open.size(); i++)
		{
			if (x == open[i].x && y == open[i].y) return  i;
		}
		return -1;
	}
	int NumIsClose(int x, int y)// номер клетки в массиве закрытого списка
	{
		for (int i = 0; i < close.size(); i++)
		{
			if (x == close [i].x && y == close[i].y) return  i;
		}
		return -1;
	}
	void RaschetInStep(int dx,int dy,PointPoisk actual)// функция расчета шага при построинии волны поиска пути
	{
		PointPoisk point;
		// присвоим point просматриваюмую клетку 
		point.x = actual.x+dx;
		point.y = actual.y +dy;
		if (IsOpen(point.x, point.y) == false)// если поинт не в эткрытом списке, тоесть ее еще не просматривали
		{
			// расчитаем F G H 
			point.H = Raschet_H(point, finish);
			point.G = actual.G + addG;
			point.F = point.G + point.H;
			// присвоем родитиля
			point.parent.x = actual.x;
			point.parent.y = actual.y;
			open.push_back(point);//добавим поинт в открытый список
		}
		else// иначе если поинт уже есть  в открытом списке
		{
			int num = NumIsOpen(point.x,point.y);// num присвоем номер клетки в открытом списке которою просматриваем
			if (actual.G + addG <= open[num].G)// если старое фактическое растояние клетки в открытом списке
				// меньше чем то которое будет если мы пойдем от выбраной клетки для расчета
			{
				// расчитаем новые F G H 
				open[num].G = actual.G + addG;
				open[num].H = Raschet_H(open[num], finish);
				open[num].F = open[num].G + open[num].H;
				// присвоем нового родителя
				open[num].parent.x = actual.x;
				open[num].parent.y = actual.y;
			}
		}
	}
	// функция распростратения волны поиска пути
	void VolnaPuti(int startX, int startY, int arrivalX, int arrivalY,RenderWindow &window, Text &text)
	{
		// сбрысываем старые данные от придыдушего поиска
		open.clear();
		close.clear();
		beingRoute = false;
		RectangleShape rectangle(sf::Vector2f(10, 10));
		PointPoisk	point, 
					actual;// клетка которую сейчас расматриваем от нее смотрим по сторонам
		
		point.x = startX / stens.size;// присваеваем поинт точку старта
		point.y = startY / stens.size;
		start.x = point.x;// иниуиализируем старт
		start.y = point.y;
		finish.x = arrivalX / stens.size;// инициализирум финиш
		finish.y = arrivalY / stens.size;
		// расчитаем для точки старта F G H
		point.G = 0;
		point.H = Raschet_H(point, finish);
		point.F = 0+Raschet_H(point,finish);
		// родитилю точки старта х у присвоем -1
		point.parent.x = -1;
		point.parent.y = -1;
		open.push_back(point);// добавим первыю точку в открытый список
		if (!(finish.x == start.x && finish.y == start.y))
		do
		{
			float minF = 100000000;
			int numI = 0;
			for (int i = 0; i < open.size(); i++)// расчет актуальной точки с намиеньшим оценкой растояния
			{
				if (open[i].F < minF)
				{
					actual = open[i];
					minF = open[i].F;
					numI = i;
				}
			}
			// удалеям из откртытого помещаем в закрытый
			close.push_back(actual);
			open.erase(open.begin()+ numI);
			// просмотр соседних клеток
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
			//		text.setColor(Color(255, 255, 255));//покрасили текст в красный. если убрать эту строку, то по умолчанию он белый
			//		std::ostringstream numRastStr;    // объявили переменную
			//		numRastStr << trunc(open[i].F);
			//		text.setString(numRastStr.str());
			//		text.setPosition(open[i].x*stens.size, open[i].y*stens.size + 8);
			//		window.draw(text);
			//	}
			//}
			//window.display();
			//Sleep(2);
			if (actual.x == finish.x && actual.y == finish.y)// если актуальная точка дошла до точки финиша
			{
				beingRoute = true;// присвоем истина тому что маршрут найден
			}
		} while (open.size()>0 && !beingRoute);// выходим из цикла если прсмотрели все доступные клетки или маршрут найден
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
	void InStepCreateRoute(PointPoisk &pointRoute, int it)// функция расчета шага при построении маршрута
	{
		PointRoute rouPo;
		rouPo.x = pointRoute.x;
		rouPo.y = pointRoute.y;
		route.push_back(rouPo);// добавим точку в маршрут 
		pointRoute.x = close[it].parent.x;// текушей точки х у присвое родятильские х у
		pointRoute.y = close[it].parent.y;
	}
	vector<PointRoute> CreateRoute(RenderWindow &window, Text &text)
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));
		PointPoisk pointRoute;// текушая точка пути
		PointRoute rouPo;
		route.clear();//очишаем маршрут
		if (beingRoute)// если маршрут найден 
		{
			// поинт присвоем точку финиша
			int numFinish = NumIsClose(finish.x,finish.y);
			int numPoint = numFinish;
			pointRoute = close[numFinish];
			do
			{
				for (int i = 0; i < close.size(); i++)//цикл по всем точкам закрытого списка
				{					
					if (pointRoute.x == close[i].x && pointRoute.y == close[i].y )// если родитель текушей точки (поинт)
						// равен х у точки закрытого списка
					{
						InStepCreateRoute(pointRoute, i);// расчитываем шаг.
						break;
					}
					
				}
				//добавим точку старта в маршрут
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
			} while ((pointRoute.x != start.x || pointRoute.y != start.y));// повторять цикл пока поинт не равен старту
		}
		reverse (route.begin(),route.end() );// инвертируем маршрут так как его строили с конца
		return route;// возрашаем маршрут
	}
	
};
// надо сделать так что если финиш приходится на препятсвие то не расчитывать волну пути