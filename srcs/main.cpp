#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

const int		widthGame = 10;
const int		heightGame = 20;
int 	field[heightGame][widthGame] = {0};

int figures[7][4] =
{
	1,5,9,13, // I
	0,1,5,6, // Z
	1,2,4,5, // S
	1,4,5,6, // T
	0,4,5,6, // L
	2,6,4,5, // J
	1,2,5,6, // O
};

struct 	Point
{
	int x, y;
};

Point	now[4], save[4];

bool	check(void)
{
   for (int i = 0; i < 4; i++)
   {
	  if (now[i].x < 0 || now[i].x >= widthGame || now[i].y >= heightGame)
	  	return 0;
      else if (field[now[i].y][now[i].x])
	  	return 0;
   }
   return 1;
}

int		checkRotate(void)
{
	for (int i = 0; i < 4; i++)
	  	if (now[i].x < 0)
			return (1);
		else if (now[i].x >= widthGame)
			return (2);
	return (0);
}

void		generatePiece(int *colorNum)
{
	rand();
	int		randPiece;
	randPiece = rand() % 7;
	(*colorNum) = 1 + randPiece;
	for (int i = 0; i < 4; i++)
	{
		int pos = figures[randPiece][i];
		while (pos > 3)
			pos = pos - 4;
		now[i].x = pos + 3;
		now[i].y = figures[randPiece][i] / 4;
	}
}

int			main(void)
{
	RenderWindow	window(VideoMode(750, 1000), "TETRIS");
	Texture			texTile, texBackground;
	texTile.loadFromFile("assets/tiles.png");
	texBackground.loadFromFile("assets/background.png");
	Sprite			sTile(texTile), sBackground(texBackground);
	float			x = (float)750 / 480;
	sBackground.setScale(x, x);
	sTile.setScale(x, x);
	int				direction = 0, colorNum = 0;
	float			timer = 0, delay = 0.3;
	bool			rotate = 0;
	Clock			clock;
	srand(time(NULL));
	rand();
	generatePiece(&colorNum);

	while (window.isOpen())
	{
		float		time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event		press;
		while (window.pollEvent(press))
		{
			if (press.type == Event::Closed)
                window.close();
			if (press.type == Event::KeyPressed)
			{
				if (press.key.code==Keyboard::Up)
					rotate=true;
			  	if (press.key.code==Keyboard::Left)
				  	direction = -1;
			  	else if (press.key.code==Keyboard::Right)
				  	direction = 1;
			  	else if (press.key.code==Keyboard::Escape)
				  	window.close();
			}
			
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.03;
		if (Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		for (int i = 0; i < 4; i++)
		{
			save[i] = now[i];
			now[i].x += direction;
		}

		if (!check())
		{
			for (int i = 0; i < 4; i++)
				now[i] = save[i];
		}
		
		if (rotate)
		{
			Point p = now[1]; //center of rotationx/
			for (int i = 0; i < 4; i++)
			{
				int x = now[i].y - p.y;
				int y = now[i].x - p.x;
				now[i].x = p.x - x;
				now[i].y = p.y + y;
			}
			if (checkRotate() > 0)
			{
				while (checkRotate() == 1)
					for (int i = 0; i < 4; i++)
						now[i].x += 1;
				while (checkRotate() == 2)
					for (int i = 0; i < 4; i++)
						now[i].x -= 1;
			}
			if (!check())
				for (int i = 0; i < 4; i++)
					now[i] = save[i];
		}

		if (timer > delay)
		{
			for (int i = 0; i < 4; i++)
			{
				save[i] = now[i];
				now[i].y += 1;
			}
			if (!check())
			{
				for (int i = 0; i < 4; i++)
					field[save[i].y][save[i].x]=colorNum;
				generatePiece(&colorNum);
			}
			timer = 0;
		}

		int k = heightGame - 1;
		for (int i = heightGame - 1; i > 0; i--)
		{
			int count= 0 ;
			for (int j = 0; j < widthGame; j++)
			{
				if (field[i][j]) 
					count++;
				field[k][j]=field[i][j];
			}
			if (count < widthGame)
				k--;
		}

		direction = 0;
		rotate = 0;
		delay=0.3;
		window.draw(sBackground);

		for (int i = 0; i < heightGame; i++)
		{
			for (int j = 0; j < widthGame; j++)
			{
				if (field[i][j] == 0)
					continue;
				sTile.setTextureRect(IntRect(field[i][j]*32,0,32,32));
				sTile.setPosition(j*50,i*50);
				window.draw(sTile);
			}
		}

		for (int i = 0; i < 4; i++)
		{
			sTile.setTextureRect(IntRect(colorNum*32,0,32,32)); // choice color
			sTile.setPosition(now[i].x * 50, now[i].y * 50);
			window.draw(sTile);
		}

		window.display();
	}
	return (0);
}
