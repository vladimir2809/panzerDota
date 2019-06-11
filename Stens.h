#include <SFML/Graphics.hpp>
using namespace sf;

class Stens// ����� ����
{

public:
	Image stensImage; //������� ������ Image (�����������)
	Texture stensTexture;//������� ������ Texture (��������)
	Sprite stensSprite;

	int size;
	enum Tip
	{
		STENA,
		VODA
	};

	struct Stena// ���� ������
	{
		int x, y;
		enum Tip tip;
	};
	Stena stena[kolvoStens];
	Stens()// ����������� ����. ������������ �������� �����
	{
		size = 40;
		stensImage.loadFromFile("City.png");//��������� � ���� ����
		stensImage.createMaskFromColor(Color(255, 255, 255));

		stensTexture.loadFromImage(stensImage);//�������� � ���� ������ Image (�����������)
		stensSprite.setTexture(stensTexture);
		stensSprite.setTextureRect(IntRect(1, 1, 40, 40));
		srand(1);
		for (int i = 0; i<kolvoStens; i++)
		{
			int rx = rand() % map_Size_X;
			int ry = rand() % map_Size_Y - 1;
			stena[i].x = rx * 40;
			stena[i].y = ry * 40;
			if (i>kolvoStens / 5 * 2) stena[i].tip = VODA; else stena[i].tip = STENA;
		}

		/*stena[0].x = 3 * 40;
		stena[0].y = 3 * 40;
		stena[0].tip = STENA;

		stena[1].x = 3 * 40;
		stena[1].y = 4 * 40;
		stena[1].tip = STENA;

		stena[2].x = 3 * 40;
		stena[2].y = 5 * 40;
		stena[2].tip = STENA;*/

	}
	void DrawStens(RenderWindow &window)// ���������� �����
	{
		RectangleShape rectangle(sf::Vector2f(size, size));
		rectangle.setFillColor(Color(128, 128, 128));
		for (int i = 0; i<kolvoStens; i++)
		{
			if (stena[i].tip == VODA) stensSprite.setTextureRect(IntRect(41, 1, 39, 39));
			if (stena[i].tip == STENA) stensSprite.setTextureRect(IntRect(1, 1, 40, 40));
			stensSprite.setPosition(stena[i].x, stena[i].y);
			window.draw(stensSprite);
		}
	}
	int InStens(int x, int y, int dx = -1, int dy = -1, bool baruer = false)// ��������� �������� ���������� � �����
	{
		for (int i = 0; i<kolvoStens; i++)
		{
			if (dx == -1 && dy == -1)
			{
				//���� �� ������ �������� dx, dy �� ��������� ���� �����
				if (baruer == false)
				{

					if (x >= stena[i].x && x <= stena[i].x + size && y >= stena[i].y && y <= stena[i].y + size)
						if (stena[i].tip == STENA)return 1; else return 2;

				}
				else
				{
					// ���� ������ ==������ �� ��������� ��������� �����
					if (x >= stena[i].x - 5 && x <= stena[i].x + size + 5 && y >= stena[i].y - 5 && y <= stena[i].y + size + 5)
						if (stena[i].tip == STENA)return 1; else return 2;
				}
			}
			else
			{
				//���� �������� dy, dx  ������� �� ��������� �� ����������� 2 ��������������
				if (x + dx >= stena[i].x && x <= stena[i].x + size && y + dy >= stena[i].y && y <= stena[i].y + size)
					if (stena[i].tip == STENA)return 1; else return 2;
			}
		}
		return 0;
	}
	int NumInStens(int x, int y, int dx, int dy )// ��������� �������� ���������� � �����
	{
		for (int i = 0; i < kolvoStens; i++)
		{
			if (x + dx >= stena[i].x && x <= stena[i].x + size && y + dy >= stena[i].y && y <= stena[i].y + size)
			{
				return i;
			}
		}
		
	}
};