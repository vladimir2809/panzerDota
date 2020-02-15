#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <math.h>
#include "functions.h"
#include "Stens.h"
#include "PoiskPuti.h"
#include "Panzer.h"
using namespace sf;
int main()
{
	sf::RenderWindow window(VideoMode(800, 600), "Dota-Panzers ");
	sf::CircleShape shape(100.f);
	Font font;
	font.loadFromFile("times-new-roman.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 10);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);//сам объект текст (не строка)

	Clock clock;
	Stens stens;
	PoiskPuti poiskPuti(stens);
	vector<PoiskPuti> arrPoiskPuti;

	//poiskPuti.VolnaPuti(10,10,20,20);

	
	PanzerData panzerData;
	vector<Panzer*> panzers;
	//Panzer* panzer = new Panzer(1,stens);
	//panzers.push_back(panzer);
	for (int i = 0; i <7; i++)
	{
		Panzer* panzer = new Panzer(1, stens, 80 + 40 / 2 - 28 / 2, (120 + 40 / 2 - 28 / 2)+i*40);
		arrPoiskPuti.push_back(poiskPuti);
		panzers.push_back(panzer);
	}
	while (window.isOpen() && !(Keyboard::isKeyPressed(Keyboard::Escape)))
	{
		bool pause = true;
		/*float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 8000;*/
		float		time = 0.5;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::LostFocus)
			{
				pause = true;

			}
			if (event.type == Event::GainedFocus)
			{
				pause = false;
			}
		}
		if (pause == true)
		{
			panzers[0]->Upravlenie(window, time);
			panzersData.clear();
			for (int i = 0; i < panzers.size(); i++)
			{
				panzerData.x = panzers[i]->x;
				panzerData.y = panzers[i]->y;
				panzerData.dx = panzers[i]->dx;
				panzerData.dy = panzers[i]->dy;
				panzerData.sizeX = panzers[i]->sizeX;
				panzerData.sizeY = panzers[i]->sizeY;
				panzerData.moving = panzers[i]->movingToRoute;
				panzerData.imEndRoute = panzers[i]->imEndRoute;
				panzerData.inMeNumCollisionpanzer = panzers[i]->numCollisionPanzer;
				panzersData.push_back(panzerData);
			}
			
			for (int i = 0; i < panzers.size(); i++)
			{
				arrPoiskPuti[i].updateXYPanzer((int)panzers[i]->x,(int)panzers[i]->y);
				//arrPoiskPuti[i].updateBarierMove();
			//	panzers[i]->Servis(time);
			}
			for (int i = 0; i < panzers.size(); i++)
			{
				panzers[i]->Servis(time);
			}
		}

		//Sleep(2);
		if (PressMouseRight(event))
		{
			Vector2i mousePos = Mouse::getPosition(window);//забираем координаты курсор
			arrPoiskPuti[0].RashetPointAim(mousePos.x / stens.size, mousePos.y / stens.size, 10);
		}
		for (int i = 0; i < panzers.size(); i++)
		{
			//panzers[i]->refreshMarshrut(window, text, poiskPuti);
			if (PressMouseLeft(event)) panzers[i]->MovingToAim(window, text, arrPoiskPuti[i]);
		}
		window.clear();
		stens.DrawStens(window);
		
		poiskPuti.DrawRoute(window,text);
		DrawNumTxt(100,570,time*1000,window,text);
		for (int i = 0; i < panzers.size(); i++)
		{
			
		//	panzers[i]->DrawMarshrut(window, text);
			panzers[i]->Draw(window, text);
			arrPoiskPuti[i].DrawPoisk(window, text);
			
		}
		for (int i = 0; i < panzers.size(); i++)
		{
			panzers[i]->DrawMarshrut(window, text);
			panzers[i]->DrawWindowOption(window,text);
		}
		window.display();
		//time = 0.1;	
		/*float time2 = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time2 = time2 / 8000;
		while (time2 - time < 10)
		{
			time = clock.getElapsedTime().asMicroseconds();
			clock.restart();
			time = time / 8000;
		}*/
	}

	return 0;
}