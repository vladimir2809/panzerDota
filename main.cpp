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
	//poiskPuti.VolnaPuti(10,10,20,20);
	Panzer* panzer = new Panzer(1,stens);
	
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 80000;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		panzer->Upravlenie(window,time);
		
		//Sleep(50);
		window.clear();
		stens.DrawStens(window);
		//poiskPuti.DrawPoisk(window, text);
		poiskPuti.DrawRoute(window,text);

		panzer->Draw(window);
		if (PressMouseLeft(event)) panzer->MovingToAim(window,text, poiskPuti);
		window.display();
	}

	return 0;
}