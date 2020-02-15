#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace sf;
using namespace std;
struct PointRoute{
		int x, y;
	};
struct PanzerData{
	int x, y, sizeY, sizeX; 
	float dx, dy;
	bool moving;
	int imEndRoute;
	bool inMeCollisionPanzer;
	int inMeNumCollisionpanzer;
};
vector<PanzerData> panzersData;
const double  pi = 3.1415926;
const int kolvoStens = 3;
int const map_Size_X = 20;
int const map_Size_Y = 14;
void DrawLine (RenderWindow &window, int x, int y, int x1, int y1, Color color)// процедура рисования линии
{
	//создаем массив точек, по которым будут рисоваться линии:
	sf::VertexArray lines(sf::Lines, 16 /*количество точек*/);

	//далее для каждой из точек задаем свою позицию:
	lines[0].position = sf::Vector2f(x, y);
	lines[1].position = sf::Vector2f(x1, y1);
	//и т.д.

	//далее для каждой точки указываем цвет(так можно создавать градиенты):
	lines[0].color = color;
	lines[1].color = color;
	//и т.д.

	//и в конце выводим все на экран:
	window.draw(lines);
}

double RaschetAngle(int x, int y, int ArrivalX, int ArrivalY)// процедура расчета угла по 2м точкам
{
	int fdx, fdy;
	double fxy, ff;
	fdx = x - ArrivalX;
	fdy = y - ArrivalY;
	fxy = fdx ? (double)fdy / fdx : 0;
	if (fdx>0) ff = atan(fxy)*180.0 / pi - 180;
	else
		ff = atan(fxy)*180.0 / pi;
	if (x == ArrivalX || fxy == 0)
	{
		if (y>ArrivalY) ff = -90;
		if (y<ArrivalY) ff = 90;
	}
	ff += 90;
	return ff;
}
double RachetRast(double x, double y, double x1, double y1)// расчет растояния между 2 точками
{
	double L = 0;// растояние
	L = sqrt((x1 - x)*(x1 - x) + (y1 - y)*(y1 - y));
	return L;
}
bool PressMouseLeft(Event event)// процедура обработки события нажатие левой кнопки мыщи
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		//if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
		//			if (event.key.code == Mouse::Left)//а именно левая
	{
		//std::cout<<"ckick mouse"<<"\n";
		return true;

	}
	return false;
}
bool PressMouseRight(Event event)// процедура обработки события нажатие левой кнопки мыщи
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		//if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
		//			if (event.key.code == Mouse::Left)//а именно левая
	{
		//std::cout<<"ckick mouse"<<"\n";
		return true;

	}
	return false;
}
double MovingToAngle(double angle, double angle1, float time, bool speedTurn = false)// функция плавного изменеия угла (нужна для того что бы прицел был плавным) 
{
	int vector = 0;
	//if (vector==0)
	{
		if (fabs(angle1 - angle) >= 180)
		{
			if (angle1>angle) vector = 2;
			if (angle1<angle) vector = 1;
		}
		else
		{
			if (angle1>angle) vector = 1;
			if (angle1<angle) vector = 2;
		}
	}
	if (angle >= 180) angle -= 360;
	if (angle <= -180) angle += 360;
	double speedRotation = 1;
	if (speedTurn == true)
	{
		if (fabs(angle1 - angle) >= 6) speedRotation = 5;
		if (fabs(angle1 - angle) >= 16) speedRotation = 10;
	}
	if (angle <= angle1 + 1 && angle >= angle1 - 1) { vector = 0; return angle; };
	if (vector == 1) return angle + speedRotation*time;
	// unit[n].f++;
	if (vector == 2) return angle - speedRotation*time;
	//unit[n].f--;


}
Vector2f BiasRotate(double pointRotateX, double pointRotateY, double angle)// смешение при врашении спрайта, длятого что бы спрайт врашался вокруг указаной точки
{
	Vector2f bias;
	double baseAngle = 0;// угол между нормали и точкой вокруг которой нужно врашать спрайт
	double totalAngle = 0;// итоговый угол сумма.
	double baseRast = 0;// растоение между 0 0 и точкой врашения спрайта
	float dx = 0, dy = 0;// смешение
	baseAngle = RaschetAngle(0, 0, pointRotateX, pointRotateY);
	baseRast = RachetRast(0, 0, pointRotateX, pointRotateY);
	totalAngle = angle + baseAngle;
	totalAngle += 180;
	dy = baseRast*sin(pi*(totalAngle - 90) / 180);
	dx = baseRast*cos(pi*(totalAngle - 90) / 180);

	dy += baseRast*sin(pi*(baseAngle - 90) / 180);
	dx += baseRast*cos(pi*(baseAngle - 90) / 180);
	bias.x = dx;
	bias.y = dy;
	return bias;
}
void DrawNumTxt(int x, int y, int mesNum, RenderWindow &window, Text &text,Color color=Color(255,255,255)){
	text.setColor(color);
	std::ostringstream numStr;    // объявили переменную
	numStr << trunc(mesNum);
	text.setString(numStr.str());
	text.setPosition(x, y);
	window.draw(text);
}
void DrawTxt(int x, int y,char message[150], RenderWindow &window, Text &text, Color color = Color(255, 255, 255))
{
	text.setColor(color);
	text.setString(message);
	text.setPosition(x, y);
	window.draw(text);
}