#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;
using namespace std;
class Panzer{
	Image panzerImage; //������� ������ Image (�����������)
	Texture panzerTexture;//������� ������ Texture (��������)
	Sprite panzerSprite;// ������ �����
	Sprite towerSprite;// ������ ����� 
	vector<PointRoute> route;// ������ ����� ��������
	//float x, y;// ����������
	int pos;// ���� ������� ����
	int vector;// ���� �������� ����
	//int sizeX, sizeY;// �������
	bool being;// �������������
	int aimMoveX, aimMoveY;
	bool normalize;// ��������� �� ����� ���� �� ����������� ������ ������
	int com;// �������
	int stepRoute;// ����� ���� ��� �������� �� ����
	float turnX, turnY, turnX1, turnY1;// ���������� ����� �����
	float angle;// ���� �������� �����
	float speed;// ��������
	
	Stens stens;// ������ ���� 
	Vector2f biasKorpus;// �������� ������� ������� ����� ��� ��������
	Vector2f biasTower;// �������� ������� ����� ��� ��������
	Vector2f biasTowerPos;// �������� ������� ����� ���� �������� ������� �����
public:
	int num;
	float x, y,dx,dy;// ����������
	int sizeX, sizeY;// �������
	bool movingToRoute;// �������� �� �a�����
    bool pauseMovintRoute;
	int imEndRoute;// � �������� �������� �� ������� 0- �� ���� ������� ��������� 1- � ��� 2 - � �������
	bool flagCollisionPanzer;
	int numCollisionPanzer;
	Panzer(int com,Stens &st,int xx=140,int yy=120)
	{
		static int  num1 = 0;
		num = num1;
		num1++;
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
		movingToRoute = false;
		pauseMovintRoute = false;
		imEndRoute = 0;
		normalize = false;
		stepRoute = 0;
		x = xx;
		y = yy;
		dx = 0;
		dy = 0;
		turnX = x + 15;
		turnY = y + 15;
		sizeX = 28;
		sizeY = 28;
		speed = 1;
		biasTowerPos.x = 7;
		biasTowerPos.y = -3;
		panzerSprite.setPosition(x,y);
		towerSprite.setPosition(x+25, y+25);
		flagCollisionPanzer = false;
		numCollisionPanzer=0;
		pos = 1;
	}
	void ChangePosition(int newPos)//�������� ���������� �����
	{
		pos = newPos;
		if (pos == 1)
		{
			panzerSprite.setRotation(0);
			biasKorpus.x = 0;
			biasKorpus.y = 0;
			angle = 0;
		}
		if (pos == 2)
		{
			panzerSprite.setRotation(90);
			
			biasKorpus.x = 28;
			biasKorpus.y = 0;
			angle = 90;
		
		}
		if (pos == 3)
		{
			panzerSprite.setRotation(180);
			biasKorpus.x = 28 - 1;
			biasKorpus.y = 29;
			angle = 180;
			
		}
		if (pos == 4)
		{
			panzerSprite.setRotation(270);
			biasKorpus.x = -2;
			biasKorpus.y = 28 - 1;
			angle = 270;
		}
		Turn(angle);
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
	void Move() // �������� ����� 
	{
		x += dx;
		y += dy;
	}
	void MoveDXDY(double dx1, double dy1) // �������� ����� 
	{
		dx = dx1;
		dy = dy1;
	}
	void CollisionStens(float time) // ������������ �� �������
	{
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
		if (y + sizeY > 560) y = 560 - sizeY;
		
	}
	bool checkCollisionPanzer()
	{
		for (int i = 0; i < panzersData.size(); i++)
		{
			if (panzersData[i].inMeCollisionPanzer == true)
			{
				return true;
			}
		}
		return false;
	}
	bool FuncNumCollisionPanzer(int numBegin=0)
	{
		for (int i = numBegin; i < panzersData.size(); i++)
		{
			if (panzersData[i].inMeCollisionPanzer == true)
			{
				return true;
			}
		}
		return false;
	}
	int checkPointCollisionPanzer(int xP,int yP)
	{
		for (int i = 0; i<panzersData.size(); i++)
		{
			if (xP>=panzersData[i].x && xP <= panzersData[i].x + panzersData[i].sizeX&&
				yP>=panzersData[i].y && yP <= panzersData[i].y + panzersData[i].sizeY)
			{
				return i;
			}
		}
		return -1;
	}
	int  numBarrierPanzer(int i=-1)// ���� �� �� ����������� �������� ����� ������ ���� ����� � ����
	{
		int xx  ;
		int yy  ;
		int sizeXX;
		int sizeYY;
		if (i != -1)
		{
			 xx = panzersData[i].x;
			 yy = panzersData[i].y;
			 sizeXX = panzersData[i].sizeX;
			 sizeYY = panzersData[i].sizeY;
		}
		else
		{
			xx = x;
			yy = y;
			sizeXX = sizeX;
			sizeYY = sizeY;
		}
		if (vector == 1)
		{
		//	if (checkPointCollisionPanzer(xx + 2, yy - 2)!=-1 ||
		//		checkPointCollisionPanzer(xx + sizeXX - 2, yy - 2) != -1)
			{
				if (checkPointCollisionPanzer(xx + 2, yy - 2) != -1)
				{
					return checkPointCollisionPanzer(xx + 2, yy - 2);
				}
				if (checkPointCollisionPanzer(xx + sizeXX - 2, yy - 2) != -1)
				{
					return checkPointCollisionPanzer(xx + sizeXX - 2, yy - 2);
				}
			}
		}
		if (vector == 2)
		{
		//	if (checkPointCollisionPanzer(xx + sizeXX + 2, yy + 2) != -1 ||
		//		checkPointCollisionPanzer(xx + sizeXX + 2, yy + sizeYY - 2) != -1)
			{
				if (checkPointCollisionPanzer(xx + sizeXX + 2, yy + 2) != -1)
				{
					return checkPointCollisionPanzer(xx + sizeXX + 2, yy + 2);
				}
				if (checkPointCollisionPanzer(xx + sizeXX + 2, yy + sizeYY - 2) != -1)
				{
					return checkPointCollisionPanzer(xx + sizeXX + 2, yy + sizeYY - 2);
				}
			}
		}
		if (vector == 3)
		{
		//	if (checkPointCollisionPanzer(xx + 2, yy + sizeYY + 2) != -1 ||
			//	checkPointCollisionPanzer(xx + sizeXX - 2, yy + sizeYY + 2) != -1)
			{
				if (checkPointCollisionPanzer(xx + 2, yy + sizeYY + 2) != -1)
				{
					return checkPointCollisionPanzer(xx + 2, yy + sizeYY + 2);
				}
				if (checkPointCollisionPanzer(xx + sizeXX - 2, yy + sizeYY + 2) != -1)
				{
					return checkPointCollisionPanzer(xx + sizeXX - 2, yy + sizeYY + 2);
				}
				
			}
		}
		if (vector == 4)
		{
		//	if (checkPointCollisionPanzer(xx - 2, yy + 2) != -1 ||
			//	checkPointCollisionPanzer(xx - 2, yy + sizeYY - 2)!=-1)
			{
				if (checkPointCollisionPanzer(xx - 2, yy + 2) != -1)
				{
					return checkPointCollisionPanzer(xx - 2, yy + 2);
				}

				if (checkPointCollisionPanzer(xx - 2, yy + sizeYY - 2) != -1)
				{
					return checkPointCollisionPanzer(xx - 2, yy + sizeYY - 2);
				}
			}
		}
		return -1;
	}
	int collisionPanzers()// ���� ���� �������� � ���� ������� ���� �� �������� �� 2 ���� �������� ������� �� 1 � ����� 0
	{
		
		/*if (checkCollisionPanzer() == true)
		{
			int k = FuncNumCollisionPanzer();
			while (k != -1)
			{
				
				if (k != -1)
				{
					bool flag = false;
					for (int i = 0; i < panzersData.size(); i++)
					{
						if (((int)panzersData[k].x == panzersData[i].x && (int)panzersData[k].y == panzersData[i].y) == false)
						{
							if	(  panzersData[k].x + panzersData[k].sizeX + panzersData[k].dx  > panzersData[i].x
								&& panzersData[k].x + panzersData[k].dx <= panzersData[i].x + panzersData[i].sizeX
								&& panzersData[k].y + panzersData[k].sizeY + panzersData[k].dy > panzersData[i].y
								&& panzersData[k].y + panzersData[k].dy <= panzersData[i].y + panzersData[i].sizeY)
								
							{
								flag = true;
								
							}
						}
					}
					if (flag ==false)
					{
						panzersData[k].inMeCollisionPanzer = false;
					}
				}
				k = FuncNumCollisionPanzer(k+1);
			} 
		}*/
		for (int i = 0; i < panzersData.size(); i++)
		{
			if (((int)x == panzersData[i].x && (int)y == panzersData[i].y) == false)
			{
					if (x + sizeX +dx > panzersData[i].x && x+dx <= panzersData[i].x + panzersData[i].sizeX
						&& y + sizeY +dy> panzersData[i].y && y+dy <= panzersData[i].y + panzersData[i].sizeY)
				
				{
					
					//if (panzersData[i].inMeCollisionPanzer == true)
				//	if (panzersData[i].moving = true)
					if (numBarrierPanzer() != -1)
					{
						x = x - dx*1;
						y = y - dy*1;
					}
					
				/*	if (panzersData[i].moving == false && imEndRoute == 2)
					{	
						return 1;
					}*/
					if (numBarrierPanzer() != -1 && panzersData[numBarrierPanzer()].imEndRoute==2)
					{
						return 1;
					}
					if (panzersData[i].moving == true && numBarrierPanzer()!=-1)
					{
						//panzersData[i].inMeCollisionPanzer = true;
					//	numCollisionPanzer = i;
						panzerSprite.setColor(Color::Red);
						return 2;
					}
				
					/*else
					{
						return 2;
					}*/
				}
			}
		}
		panzerSprite.setColor(Color::White);
		return 0;
	}
	bool NormalizeX(float time)// ��������� �� ����� ����� �� ����������� ����� ������ �� �
	{
		float xx =x;
		for (int i = 0; i < map_Size_X; i++)
		{
			if (xx + (int)(sizeX / 2) < i*stens.size + stens.size / 2 + speed*time * 2 &&
				xx + (int)(sizeX / 2) > i*stens.size + stens.size / 2 - speed*time * 2)
				return true;
		}
		return false;
	}
	bool NormalizeY(float time)// ��������� �� ����� ����� �� ����������� ����� ������ �� �
	{
		float yy = y;
		for (int i = 0; i < map_Size_Y; i++)
		{
			if (yy + (int)(sizeY / 2)  < i*stens.size + stens.size / 2 + speed*time *2&&
				yy + (int)(sizeY / 2)  > i*stens.size + stens.size / 2 - speed*time*2 )
				return true;
		}
		return false;
	}
	void MoveToVector(float time)// �������� �� �������
	{
		if (vector == 1 && pos == 1)
		{
			MoveDXDY(0, -speed*time);
		}
		if (vector == 2 && pos == 2)
		{
			MoveDXDY(speed*time, 0);
		}
		if (vector == 3 && pos == 3)
		{
			MoveDXDY(0, speed*time);
		}
		if (vector == 4 && pos == 4)
		{
			MoveDXDY(-speed*time, 0);
		}
		if (vector == 1) pos = 1;
		if (vector == 2) pos = 2;
		if (vector == 3) pos = 3;
		if (vector == 4) pos = 4;
		CollisionStens(time);
	}
	void NormalizeMovingToRouteX(float time)//������������� ���� �� ������ ������ �� �
	{
		float xx = x;
		if (route.size()>0)
		if (NormalizeX(time) == false)
		{
			if ((xx + (int)sizeX / 2 <= route[0].x*stens.size + stens.size / 2)) 
			{
				vector = 2;
			}
			else
			{
				vector = 4;
			}
		}
		else
		{
			vector = 0;
		}
	}
	void NormalizeMovingToRouteY(float time)//������������� ���� �� ������ ������ �� �
	{
		float yy = y;
		if (route.size()>0)
		if (NormalizeY(time) == false)
		{
			if ((yy + (int)sizeY / 2 <= route[0].y*stens.size + stens.size / 2)) 
			{
				vector = 3;
			}
			else
			{
				vector = 1;
			}
		}
		else
		{
			vector = 0;
		}
	}
	void NormalizeToRoute(float time)// ������������� ���� ��� ����� ����� ���� ��������� �� �������� �� �������� � �����
	{
		if (route.size() > 0)
		{
			if (route[0].y == route[1].y)
				if ((route[0].x > route[1].x || route[0].x < route[1].x))
				{
					NormalizeMovingToRouteY(time);
				}
			if (route[0].x == route[1].x)
				if ((route[0].y > route[1].y || route[0].y < route[1].y))
				{
					NormalizeMovingToRouteX(time);
				}

		}
	}
	/////////////////////////////////////////////////////////////////
	bool FlagNormalizeToRoute(float time)	// ��������� ������ ���� ���������� ����� �������� ��� �������� �� ��������
	{
		if (route.size() > 0)
		{
			if (route[0].y == route[1].y)
				if ((route[0].x > route[1].x || route[0].x < route[1].x))
				{
					if (NormalizeY(time)) 
						return true;
				}
			if (route[0].x == route[1].x)
				if ((route[0].y > route[1].y || route[0].y < route[1].y))
				{
					if (NormalizeX(time)) return true;
				}

		}
		return false;
	}
	/////////////////////////////////
	void CancelRoute()// �������� �������
	{
		movingToRoute = false;
		route.clear();
		vector = 0;
		stepRoute = 0;
		normalize = false;
		dx = 0;
		dy = 0;
	}
	void MovingToRoute(float time )// �������� �� ��������
	{
		
		int xx = trunc(x);
		int yy = trunc(y);
		
		/*	float xx = x;
		float yy = y;*/
		// ��������� ���������� ������ �����
		int centerPanzerX = xx + (int)(sizeX / 2);
		int centerPanzerY = yy + (int)(sizeY / 2);
		if (normalize == false) NormalizeToRoute(time);// ���� ���� �� ������������ �� ���������������
		if (FlagNormalizeToRoute(time)) normalize = true;// ���� ����� ����� ������������
		/////////////////////////////////////////// ������� ������ �������� �� ���������
		if (movingToRoute==true)
		if (route.size()>0)
		if (pauseMovintRoute==false)
		if (normalize==true)
		{
			// ��������� ����� ������ � ������� ���� ����
			int centerStepX = route[stepRoute + 1].x*stens.size + stens.size / 2;
			int centerStepY = route[stepRoute + 1].y*stens.size + stens.size / 2;
			// ���� ����� � ����� ����� ������ ������ �������� ���� �� �
			if (centerPanzerX > centerStepX - speed*time * 2 && centerPanzerX < centerStepX + speed*time * 2)
			{
				if (centerPanzerY > centerStepY)//���� ���� ���� ������ �� ������� ����
				{
					vector = 1;
					ChangePosition(1);
				}

				if (centerPanzerY < centerStepY)//���� ���� ���� ������ �� ������� ����
				{
					vector = 3;
					ChangePosition(3);
				}
			}
			// ���� ����� � ����� ����� ������ ������ � ������ ���� �� �
			if (centerPanzerY > centerStepY - speed*time * 2 && centerPanzerY < centerStepY + speed*time * 2)
			{
				if (centerPanzerX > centerStepX)// ���� ���� ������
				{
					vector = 4;
					ChangePosition(4);
				}
				if (centerPanzerX < centerStepX)// ���� ���� �����
				{
					vector = 2;
					ChangePosition(2);
				}
			}
			// ���� ����� ����� ����� ������ ������ 
			if (centerPanzerX < centerStepX + speed*time*2  && centerPanzerX > centerStepX - speed*time*2
				&& centerPanzerY < centerStepY + speed*time*2  && centerPanzerY > centerStepY - speed*time*2 )
			{
				/*do
				{
					x = x;
				}while(Keyboard::isKeyPressed(Keyboard::Tab)==false);
*/
				/*while (1)
				{
					if (Keyboard::isKeyPressed(Keyboard::Tab)) break;
				}*/
				if (stepRoute < route.size()-2)// ���� ����� ���� ������ ����� ��������
				{
						stepRoute++;	
				}
				else// ����� ���� ����� ���� ����� ����� �������� 
				{
					imEndRoute = 2;
					CancelRoute();// �������� �������
				}
			}
			if (collisionPanzers() == 2)// ���� ���� �������� � ���� ������� ���� �� ��������
			{
			//	if (numBarrierPanzer()==-1)
					pauseMovintRoute = true;
			}
			if (collisionPanzers() == 1)// ���� ���� �������� � ���� ������� ������� � ������
			{
				imEndRoute = 2;
				CancelRoute();// �������� �������
			}
		}
		if (collisionPanzers() == 0)// ���� ���� �� ��������� � ������ ����
		{
			pauseMovintRoute = false;
		}
	}
	void Upravlenie(RenderWindow &window, float time)// ���������� � �����������
	{
		double angleMouse; // ���� �������� ���� ������� ����
		
		if (Keyboard::isKeyPressed(Keyboard::W) && pos == 1) // �������� �����
		{

				MoveDXDY(0, -speed*time);
				Move();

		}
		if (Keyboard::isKeyPressed(Keyboard::D) && pos == 2) // �������� ������
		{
			
			MoveDXDY(speed*time, 0);
			Move();
		}
		if (Keyboard::isKeyPressed(Keyboard::S) && pos == 3) // �������� ����
		{
			MoveDXDY(0, speed*time);
			Move();
		}
		if (Keyboard::isKeyPressed(Keyboard::A) && pos == 4) // ������� �����
		{
			
			MoveDXDY(-speed*time, 0);
			Move();
		}
		MoveDXDY(0, 0);
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
	void refreshMarshrut(RenderWindow &window, Text &text, PoiskPuti poiskPuti)
	{
		if (movingToRoute == true)
		{
			poiskPuti.VolnaPuti(x + sizeX / 2, y + sizeY / 2, aimMoveX, aimMoveY, window, text);
			route = poiskPuti.CreateRoute(window, text);//��������� � �������� ������� 
			//if (route.size() > 0) movingToRoute = true;
		}
	}
	void MovingToAim(RenderWindow &window,Text &text, PoiskPuti poiskPuti)// ������ ������� �����
	{
		CancelRoute();// ������� ������ �������
		if (movingToRoute == false)
		{
			Vector2i mousePos = Mouse::getPosition(window);//�������� ���������� �������
			
			aimMoveX = mousePos.x;
			aimMoveY= mousePos.y;
			//movingToRoute = true;
			// ��������������� ����� ���� 
			poiskPuti.VolnaPuti(x + sizeX / 2, y + sizeY / 2, mousePos.x, mousePos.y, window, text);
			route = poiskPuti.CreateRoute(window, text);//��������� � �������� ������� 
			if (route.size() > 0)
			{
				movingToRoute = true;
				imEndRoute = 1;
			}
		}
	}
	void Servis(float time)// ������� ������� �����, ����� ����������� �� ������� ������� ������ ����������� � ����� 
	{
		
		if (Keyboard::isKeyPressed(Keyboard::Num0) && num == 0)
		{
			x = x;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num1) && num==1)
		{
			x = x;
		}
		if (Keyboard::isKeyPressed(Keyboard::Num2) && num == 2)
		{
			x = x;
		}
		/*while (1)
		{
			if (Keyboard::isKeyPressed(Keyboard::Tab)) break;
		}	*/	
		MoveToVector(time);
		MovingToRoute(time);
		Move();
		//if (collisionPanzers() != 2)
		
	}
	void DrawWindowOption(RenderWindow &window,Text &text)
	{
		RectangleShape rectangle(sf::Vector2f(120, 70));
		rectangle.setFillColor(Color::White);
		Vector2i mousePos = Mouse::getPosition(window);//�������� ���������� �������
		if (mousePos.x > x && mousePos.x < x + sizeX&&
			mousePos.y > y && mousePos.y < y + sizeY)
		{
			rectangle.setPosition(mousePos.x,mousePos.y-70);
			window.draw(rectangle);
			int xx = mousePos.x+2;
			int yy = mousePos.y-70;
			DrawTxt(xx, yy, "imEndRoute", window, text, Color::Black);
			DrawNumTxt(xx+60, yy, imEndRoute, window, text, Color::Black);
			DrawTxt(xx, yy+10, "moving", window, text, Color::Black);
			DrawNumTxt(xx + 60, yy+10,movingToRoute==true? 1:0, window, text, Color::Black);
			DrawTxt(xx, yy+20, "vector", window, text, Color::Black);
			DrawNumTxt(xx + 60, yy+20, vector, window, text, Color::Black);
		}

	}
	void DrawMarshrut(RenderWindow &window, Text &text)
	{
		RectangleShape rectangle(sf::Vector2f(3, 3));
		for (int i = 0; i < route.size(); i++)// ������� ����������� move
		{
			//if (num==0)
			{
				rectangle.setFillColor(Color(255, 125+num*35, 0+num*35));
				rectangle.setPosition(route[i].x*stens.size + 15, route[i].y*stens.size + 15);
				window.draw(rectangle);
			}
		}
	}
	void Draw(RenderWindow &window,Text &text)// ������� ��������� �����
	{
		panzerSprite.setPosition(x + biasKorpus.x, y + biasKorpus.y);
		towerSprite.setPosition(x + biasTowerPos.x + biasTower.x, y + biasTowerPos.y + biasTower.y);
		//DrawNumTxt(x, y- 12, stepRoute, window, text);
		DrawNumTxt(x, y-10, num, window, text);
		window.draw(panzerSprite);
		window.draw(towerSprite);
	}
};