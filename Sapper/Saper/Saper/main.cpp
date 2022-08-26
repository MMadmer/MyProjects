#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include "globals.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <iterator>
#include <sstream>
#include "cell.h"

using namespace std;
using namespace sf;

vector<cell*> currentCell;
vector<cell*>::iterator it;

int height = 432; //������ ����
int widht = 432; //������ ����
int h = 9; //������������ ������ ����
int w = 9; //�������������� ������ ����
bool leftPressed = false;
bool rightPressed = false;

int Map[16][30];

int getRandomNumber(int min, int max);
void game();
int choiceMap();
void drawField(RenderWindow &window);
void closeCells(int i);
void newGame(int &mines, int chance);
void openCells(int i);
void autoOpenCells(int xM, int yM, bool z);
void countFlagsAbout(int i, int p);
Font font;

int main(int &argc, char argv)
{
	font.loadFromFile("assets/Context_Reprise_SSi.ttf");

	for (int i = 16 * 30; i > 0; i--) currentCell.push_back(new cell());

	game();
	
return 0;
}

void game()
{
	int x = 39;
	int y = 19;
	Event event;
	cell icon; //������ ��� ���������� ���

	icon.decorate = true;
	icon.drawCells();
	icon.sprite.setPosition(widht / 2 - 25, height + 10);

	//�������� ����
	RenderWindow window(VideoMode(widht, height + 50), "Sapper");
	Text text("", font, 25);
	text.setPosition(widht / 2 + 5, height + 8);

	window.setFramerateLimit(30);
	/*window.setVerticalSyncEnabled(true);*/

	while ((window.isOpen()) && (!Keyboard::isKeyPressed(Keyboard::Escape)))
	{
		while (window.pollEvent(event))
		{
			if (event.type == Event::MouseButtonReleased)
			{
				if (event.key.code == Mouse::Right) rightPressed = true;
				if (event.key.code == Mouse::Left) leftPressed = true;
			}
			if (event.type == event.Closed)
				window.close();
		}

		//�����
		ostringstream string_of_mines;
		string_of_mines << mines;
		text.setString(string_of_mines.str());

		if (nowNumberMap == 1 && lastNumberMap == 3)
		{
			lastNumberMap = 1;
			window.close();
			game();
		}
		if (nowNumberMap == 2 && lastNumberMap == 3)
		{
			lastNumberMap = 2;
			window.close();
			game();
		}
		if (nowNumberMap == 3 && lastNumberMap < 3)
		{
			lastNumberMap = 3;
			window.close();
			game();
		}

		//���� ��� �������
		srand(static_cast<unsigned int>(time(0)));
		//����� ���� � ���������� ������
		nowNumberMap = choiceMap();
		
		drawField(window);

		window.draw(text);
		window.draw(icon.sprite);

		window.display();
	}
}

void drawField(RenderWindow &window)
{
	it = currentCell.begin();
	window.clear(Color::Blue);
	if (win == false)
	{
		if (voidCells == clickedVoidCells)
		{
			win = true;
			mines = 0;
		}
	}
	//��������� ��� � �����
	for (int yM = 0; yM < h; yM++)
		for (int xM = 0; xM < w; xM++)
		{
			
			if(win) (*it)->flag = false;

			//�������� ������ ������ ������� �������� ������
			if ((*it)->numberCell == 0 && (*it)->opened && (*it)->used == false) autoOpenCells(xM, yM, true);

			//��������� ������
			(*it)->setCellParameters(xM * (widht / w), yM * (height / h));

				//������ ������ ����
			if (rightPressed)
			{
				Vector2i pixelPos = Mouse::getPosition(window); //���������� ��������� �������

				if ((*it)->sprite.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
				{
					//��������� ����
					if ((*it)->flag == false && (*it)->opened == false && rightPressed && win == false && mines > 0)
					{
						(*it)->flag = true;
						rightPressed = false;
						mines--;

						//���������� ���������� ������ ������� �� 1 ��� ���� ����� ������ �����
						if (yM > 0) countFlagsAbout(-w, 1);
						if (yM < (h - 1)) countFlagsAbout(w, 1);
						if (xM > 0) countFlagsAbout(-1, 1);
						if (xM < (w - 1)) countFlagsAbout(1, 1);
						if (yM > 0 && xM > 0) countFlagsAbout(-w - 1, 1);
						if (yM > 0 && xM < (w - 1)) countFlagsAbout(-w + 1, 1);
						if (yM < (h - 1) && xM > 0) countFlagsAbout(w - 1, 1);
						if (yM < (h - 1) && xM < (w - 1)) countFlagsAbout(w + 1, 1);
					}
					//������ ����
					if ((*it)->flag && (*it)->opened == false && rightPressed && win == false)
					{
						(*it)->flag = false;
						rightPressed = false;
						mines++;

						//���������� ���������� ������ ������� �� 1 ��� ���� ����� ������ �����
						if (yM > 0) countFlagsAbout(-w, -1);
						if (yM < (h - 1)) countFlagsAbout(w, -1);
						if (xM > 0) countFlagsAbout(-1, -1);
						if (xM < (w - 1)) countFlagsAbout(1, -1);
						if (yM > 0 && xM > 0) countFlagsAbout(-w - 1, -1);
						if (yM > 0 && xM < (w - 1)) countFlagsAbout(-w + 1, -1);
						if (yM < (h - 1) && xM > 0) countFlagsAbout(w - 1, -1);
						if (yM < (h - 1) && xM < (w - 1)) countFlagsAbout(w + 1, -1);
					}
					rightPressed = false;
				}
			}
				//����� ������ ����
			if (leftPressed)
			{
				Vector2i pixelPos = Mouse::getPosition(window); //���������� ��������� �������

				if ((*it)->sprite.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
				{
					if ((win == false) && ((*it)->flag == false))
					{
						if ((*it)->opened && (*it)->numberCell > 0 && (*it)->numberCell == (*it)->flagsAbout) autoOpenCells(xM, yM, false);
						if ((*it)->isMine) fail = true; //���� ����, �� �������� � �������� ���� �����
						else if ((*it)->opened == false && (*it)->isMine == false)
						{ //���� �� ����, �� ������ ����� ������ � ����� �������
							(*it)->opened = true;
							clickedVoidCells++;
						}
					}
					leftPressed = false;
				}
			}

			(*it)->drawCells();
			window.draw((*it)->sprite);
			
			it++;
		}

	//��������� �����
	for (int i = widht / w; i < widht; i += widht / w)
	{
		Vertex line[] =
		{
			Vertex(Vector2f(i, 0)),
			Vertex(Vector2f(i, height))
		};
		window.draw(line, 2, Lines);
	}
	for (int i = height / h; i < height; i += height / h)
	{
		Vertex line[] =
		{
			Vertex(Vector2f(0, i)),
			Vertex(Vector2f(widht, i))
		};
		window.draw(line, 2, Lines);
	}
}

int choiceMap() {
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		h = w = 9;
		mines = 10;
		height = 432;
		widht = 432;

		newGame(mines, 7);

		return 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2))
	{
		h = w = 16;
		mines = 40;
		height = 432;
		widht = 432;

		newGame(mines, 7);

		return 2;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num3))
	{
		h = 16;
		w = 30;
		mines = 99;
		height = 432;
		widht = 600;

		newGame(mines, 5);

		return 3;
	}
}

void closeCells(int i)
{
	it = currentCell.begin();
	for (i; i > 0; i--)
	{
		(*it)->opened = false;
		(*it)->flag = false;
		(*it)->used = false;
		(*it)->flagsAbout = 0;
		(*it)->numberCell = 0;
		it++;
	}
	it = currentCell.begin();
}

void openCells(int i)
{
	advance(it, i);
	if ((*it)->flag == false)
	{
		if ((*it)->opened == false)
		{
			(*it)->opened = true;
			if ((*it)->isMine) fail = true;
			else clickedVoidCells++;
		}
	}
	advance(it, -i);
}

void autoOpenCells(int xM, int yM, bool z)
{
	if (yM > 0) openCells(-w); //�������� ������ ������
	if (yM < (h - 1)) openCells(w); //�������� ������ �����
	if (xM > 0) openCells(-1); //�������� ������ �����
	if (xM < (w - 1)) openCells(1); //�������� ������ ������
	if (yM > 0 && xM > 0) openCells(-w - 1); //�������� ������ ����� ������
	if (yM > 0 && xM < (w - 1)) openCells(-w + 1); //�������� ������ ������ ������
	if (yM < (h - 1) && xM > 0) openCells(w - 1); //�������� ������ ����� �����
	if (yM < (h - 1) && xM < (w - 1)) openCells(w + 1); //�������� ������ ������ �����
	if (z)(*it)->used = true;
}

void countFlagsAbout(int i, int p)
{
	advance(it, i);
	(*it)->flagsAbout += p;
	advance(it, -i);
}

void newGame(int &mines, int chance)
{
	int minesCount = 0; //��� �� ����
	int i = 0;
	win = false;
	fail = false;
	voidCells = h * w - mines;
	clickedVoidCells = 0;

	closeCells(h * w);

	for (int yM = 0; yM < 16; yM++)
		for (int xM = 0; xM < 30; xM++)
			Map[yM][xM] = 0;

	do
	{
		for (int yM = 0; yM < h; yM++)
			for (int xM = 0; xM < w; xM++)
			{
				if (Map[yM][xM] == 1) break;
				else
				{
					i = getRandomNumber(0, chance);
					if (i == 1 && minesCount < mines)
					{
						Map[yM][xM] = 1;
						minesCount++;
					}
				}
			}
	} while (minesCount < mines);

	int count = 0;
	it = currentCell.begin();
	for (int yM = 0; yM < h; yM++)
		for (int xM = 0; xM < w; xM++)
		{
			//������ ��� ������
			if (Map[yM][xM] == 0)
			{
				(*it)->isMine = false;

				if (yM > 0 && Map[yM - 1][xM] == 1) (*it)->numberCell++; //�������� ���� ������
				if (yM < (h - 1) && Map[yM + 1][xM] == 1) (*it)->numberCell++; //�������� ���� �����
				if (xM > 0 && Map[yM][xM - 1] == 1) (*it)->numberCell++; //�������� ���� �����
				if (xM < (w - 1) && Map[yM][xM + 1] == 1) (*it)->numberCell++; //�������� ���� ������
				if (yM > 0 && xM > 0 && Map[yM - 1][xM - 1] == 1) (*it)->numberCell++; //�������� ���� ����� ������
				if (yM > 0 && xM < (w - 1) && Map[yM - 1][xM + 1] == 1) (*it)->numberCell++; //�������� ���� ������ ������
				if (yM < (h - 1) && xM > 0 && Map[yM + 1][xM - 1] == 1) (*it)->numberCell++; //�������� ���� ����� �����
				if (yM < (h - 1) && xM < (w - 1) && Map[yM + 1][xM + 1] == 1) (*it)->numberCell++; //�������� ���� ������ �����
			}
			else
			{
				(*it)->isMine = true; count++;
			}
			it++;
		}
	it = currentCell.begin();
}

int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	//������������� �����
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
