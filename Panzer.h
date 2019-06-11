#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
class Panzer{
	Image panzerImage; //������� ������ Image (�����������)
	Texture panzerTexture;//������� ������ Texture (��������)
	Sprite panzerSprite;
	Sprite towerSprite;
	vector<PointRoute> route;
	float x, y;
	int pos;
	int sizeX, sizeY;
	bool being;
	int com;
	float turnX, turnY, turnX1, turnY1;
	float angle;
	float speed;
	Stens stens;
	Vector2f biasKorpus;// �������� ������� ������� ����� ��� ��������
	Vector2f biasTower;// �������� ������� ����� ��� ��������
	Vector2f biasTowerPos;// �������� ������� ����� ���� �������� ������� �����
public:
	Panzer(int com,Stens &st){
		panzerImage.loadFromFile("Panzers.png");//��������� � ���� ����
		panzerImage.createMaskFromColor(Color(255, 255, 255));
		panzerImage.createMaskFromColor(Color(0, 0, 0));

		panzerTexture.loadFromImage(panzerImage);//�������� � ���� ������ Image (�����������)
		panzerSprite.setTexture(panzerTexture);
		towerSprite.setTexture(panzerTexture);
		panzerSprite.setTextureRect(IntRect(1, 1, 40, 40));
		towerSprite.setTextureRect(IntRect(41, 1, 39, 39));
		stens = st;
		angle = 0;
		
		x = 140;
		y = 100;
		turnX = x + 15;
		turnY = y + 15;
		sizeX = 28;
		sizeY = 28;
		speed = 10;
		biasTowerPos.x = 7;
		biasTowerPos.y = -3;
		panzerSprite.setPosition(x,y);
		towerSprite.setPosition(x+25, y+25);
		pos = 1;
	}
	void ChangePosition(int newPos)
	{
		pos = newPos;
		if (pos == 1)
		{
			panzerSprite.setRotation(0);
			biasKorpus.x = 0;
			biasKorpus.y = 0;
			
		}
		if (pos == 2)
		{
			panzerSprite.setRotation(90);
			
			biasKorpus.x = 28;
			biasKorpus.y = 0;
		
		}
		if (pos == 3)
		{
			panzerSprite.setRotation(180);
			biasKorpus.x = 28 - 1;
			biasKorpus.y = 29;
			
		}
		if (pos == 4)
		{
			panzerSprite.setRotation(270);
			biasKorpus.x = -2;
			biasKorpus.y = 28 - 1;
		}
	}
	void Turn(double angle)// ������� ����� � ������������ ����
	{
		//int size1=size/3;
		turnY = sizeY * sin(pi*(angle - 90) / 180) + y + (int)sizeY/2;
		turnX = sizeX * cos(pi*(angle - 90) / 180) + x + (int)sizeX / 2;
		turnY1 = sizeY*sin(pi*(angle - 90) / 180) + y + (int)sizeY / 2;
		turnX1 = sizeX*cos(pi*(angle - 90) / 180) + x + (int)sizeX / 2;
		towerSprite.setRotation(angle);
		biasTower = BiasRotate(6, 17, angle);
	}
	void Move(double dx, double dy) // �������� ����� 
	{
		x += dx;
		y += dy;

	}
	void CollisionStens(float time){
		int numSt = 0;
		if (stens.InStens(x, y, sizeX, sizeY) != 0)
		{
			numSt = stens.NumInStens(x, y, sizeX, sizeY);
			if (pos == 1){
				y = stens.stena[numSt].y + stens.size+1;
			}
			if (pos == 2){
				x = stens.stena[numSt].x - sizeX - speed*time;
			}
			if (pos == 3){
				y = stens.stena[numSt].y - sizeY - speed*time;
			}
			if (pos == 4){
				x = stens.stena[numSt].x + stens.size + 1;
			}
		}
		if (x < 0) x = 1;
		if (y < 0) y = 1;
		if (x + sizeX > 800) x = 800 - sizeX;
		if (y + sizeY > 600) y = 600 - sizeY;
		
	}
	void Upravlenie(RenderWindow &window, float time){
		double angleMouse; // ���� �������� ���� ������� ����
		
		if (Keyboard::isKeyPressed(Keyboard::W) && pos == 1) // �������� �����
		{
			
				Move(0, -speed*time);
		
		}
		if (Keyboard::isKeyPressed(Keyboard::D) && pos == 2) // �������� ������
		{
			
			Move(speed*time, 0);
			
		}
		if (Keyboard::isKeyPressed(Keyboard::S) && pos == 3) // �������� ����
		{
			Move(0, speed*time);
			
		}
		if (Keyboard::isKeyPressed(Keyboard::A) && pos == 4) // ������� �����
		{
			
			Move(-speed*time, 0);
			
		}
		CollisionStens(time);
		if (Keyboard::isKeyPressed(Keyboard::W)) ChangePosition(1); // ������� �����
		if (Keyboard::isKeyPressed(Keyboard::D)) ChangePosition(2); // ������� ������
		if (Keyboard::isKeyPressed(Keyboard::S)) ChangePosition(3); // ������� ����
		if (Keyboard::isKeyPressed(Keyboard::A)) ChangePosition(4); // ������� �����

		Vector2i mousePos = Mouse::getPosition(window);//�������� ���������� �������

		angleMouse = RaschetAngle(x + (int)sizeX/2, y + (int)sizeY/2, mousePos.x, mousePos.y);// ������ ���� ����
		angle = MovingToAngle(angle, angleMouse,time);
		Turn(angle);
	}
	void MovingToAim(RenderWindow &window,Text &text, PoiskPuti &poiskPuti)
	{
		Vector2i mousePos = Mouse::getPosition(window);//�������� ���������� �������
		poiskPuti.VolnaPuti(x + sizeX / 2, y + sizeY / 2, mousePos.x, mousePos.y, window,text);
		route = poiskPuti.CreateRoute(window, text);
	}
	void Servis(float time){

	}
	void Draw(RenderWindow &window){
		panzerSprite.setPosition(x + biasKorpus.x, y + biasKorpus.y);
		//panzerSprite.setRotation(45);
		towerSprite.setPosition(x + biasTowerPos.x + biasTower.x, y + biasTowerPos.y + biasTower.y);

		window.draw(panzerSprite);
		window.draw(towerSprite);
	}
};