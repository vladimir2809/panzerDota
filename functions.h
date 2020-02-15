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
void DrawLine (RenderWindow &window, int x, int y, int x1, int y1, Color color)// ��������� ��������� �����
{
	//������� ������ �����, �� ������� ����� ���������� �����:
	sf::VertexArray lines(sf::Lines, 16 /*���������� �����*/);

	//����� ��� ������ �� ����� ������ ���� �������:
	lines[0].position = sf::Vector2f(x, y);
	lines[1].position = sf::Vector2f(x1, y1);
	//� �.�.

	//����� ��� ������ ����� ��������� ����(��� ����� ��������� ���������):
	lines[0].color = color;
	lines[1].color = color;
	//� �.�.

	//� � ����� ������� ��� �� �����:
	window.draw(lines);
}

double RaschetAngle(int x, int y, int ArrivalX, int ArrivalY)// ��������� ������� ���� �� 2� ������
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
double RachetRast(double x, double y, double x1, double y1)// ������ ��������� ����� 2 �������
{
	double L = 0;// ���������
	L = sqrt((x1 - x)*(x1 - x) + (y1 - y)*(y1 - y));
	return L;
}
bool PressMouseLeft(Event event)// ��������� ��������� ������� ������� ����� ������ ����
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		//if (event.type == Event::MouseButtonPressed)//���� ������ ������� ����
		//			if (event.key.code == Mouse::Left)//� ������ �����
	{
		//std::cout<<"ckick mouse"<<"\n";
		return true;

	}
	return false;
}
bool PressMouseRight(Event event)// ��������� ��������� ������� ������� ����� ������ ����
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		//if (event.type == Event::MouseButtonPressed)//���� ������ ������� ����
		//			if (event.key.code == Mouse::Left)//� ������ �����
	{
		//std::cout<<"ckick mouse"<<"\n";
		return true;

	}
	return false;
}
double MovingToAngle(double angle, double angle1, float time, bool speedTurn = false)// ������� �������� �������� ���� (����� ��� ���� ��� �� ������ ��� �������) 
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
Vector2f BiasRotate(double pointRotateX, double pointRotateY, double angle)// �������� ��� �������� �������, ������� ��� �� ������ �������� ������ �������� �����
{
	Vector2f bias;
	double baseAngle = 0;// ���� ����� ������� � ������ ������ ������� ����� ������� ������
	double totalAngle = 0;// �������� ���� �����.
	double baseRast = 0;// ��������� ����� 0 0 � ������ �������� �������
	float dx = 0, dy = 0;// ��������
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
	std::ostringstream numStr;    // �������� ����������
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