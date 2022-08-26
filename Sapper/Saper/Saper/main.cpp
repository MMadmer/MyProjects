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

int windowHeight = 432; // ������ ����
int windowWidht = 432; // ������ ����
int rows = 9; // ������������ ������ ����
int columns = 9; // �������������� ������ ����
bool leftPressed = false;
bool rightPressed = false;

int Map[16][30];

int GetRandomNumber(int min, int max);
void Game();
int ChooseMap();
void CloseCells(int i);
void NewGame(int &mines, int chance);
void OpenCells(int i);
void AutoOpenCells(int xM, int yM, bool z);
void CountFlagsAbout(int i, int p);
void DrawField(RenderWindow& window);
void SetWindowBackground(RenderWindow &window);
void GridDraw(RenderWindow& window);
void DrawCells(RenderWindow &window);

Font font;

int main(int &argc, char argv)
{
	font.loadFromFile("assets/Context_Reprise_SSi.ttf");

	for (int i = 16 * 30; i > 0; i--) currentCell.push_back(new cell());

	Game();
	
return 0;
}

void Game()
{
	int x = 39;
	int y = 19;
	Event event;
	cell icon; // ������ ��� ���������� ���

	icon.decorate = true;
	icon.DrawCells();
	icon.sprite.setPosition(windowWidht / 2 - 25, windowHeight + 10);

	// �������� ����
	RenderWindow window(VideoMode(windowWidht, windowHeight + 50), "Sapper");
	Text text("", font, 25);
	text.setPosition(windowWidht / 2 + 5, windowHeight + 8);

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

		// �����
		ostringstream string_of_mines;
		string_of_mines << gMines;
		text.setString(string_of_mines.str());

		if (gNowNumberMap == 1 && gLastNumberMap == 3)
		{
			gLastNumberMap = 1;
			window.close();
			Game();
		}
		if (gNowNumberMap == 2 && gLastNumberMap == 3)
		{
			gLastNumberMap = 2;
			window.close();
			Game();
		}
		if (gNowNumberMap == 3 && gLastNumberMap < 3)
		{
			gLastNumberMap = 3;
			window.close();
			Game();
		}

		// ���� ��� �������
		srand(static_cast<unsigned int>(time(0)));
		// ����� ���� � ���������� ������
		gNowNumberMap = ChooseMap();
		
		DrawField(window);

		window.draw(text);
		window.draw(icon.sprite);

		window.display();
	}
}

//////////////////////////////////////////////////////////////////////////////
// ��������� ���� REGION BEGIN
//////////////////////////////////////////////////////////////////////////////
void DrawField(RenderWindow &window)
{
	// �������� ����
	SetWindowBackground(window);

	if (gWin == false)
	{
		if (voidCells == clickedVoidCells)
		{
			gWin = true;
			gMines = 0;
		}
	}

	// ��������� ��� � �����
	DrawCells(window);

	// ��������� �����
	GridDraw(window);
}

void SetWindowBackground(RenderWindow& window)
{
	it = currentCell.begin();
	window.clear(Color::Blue);
}

void GridDraw(RenderWindow &window)
{
	for (int i = windowWidht / columns; i < windowWidht; i += windowWidht / columns)
	{
		Vertex line[] =
		{
			Vertex(Vector2f(i, 0)),
			Vertex(Vector2f(i, windowHeight))
		};
		window.draw(line, 2, Lines);
	}
	for (int i = windowHeight / rows; i < windowHeight; i += windowHeight / rows)
	{
		Vertex line[] =
		{
			Vertex(Vector2f(0, i)),
			Vertex(Vector2f(windowWidht, i))
		};
		window.draw(line, 2, Lines);
	}
}

void DrawCells(RenderWindow &window)
{
	for (int yM = 0; yM < rows; yM++)
		for (int xM = 0; xM < columns; xM++)
		{

			if (gWin) (*it)->flag = false;

			// �������� ������ ������ ������� �������� ������
			if ((*it)->numberCell == 0 && (*it)->opened && (*it)->used == false) AutoOpenCells(xM, yM, true);

			// ��������� ������
			(*it)->SetCellParameters(xM * (windowWidht / columns), yM * (windowHeight / rows));

			// ������ ������ ����
			if (rightPressed)
			{
				Vector2i pixelPos = Mouse::getPosition(window); // ���������� ��������� �������

				if ((*it)->sprite.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
				{
					// ��������� ����
					if ((*it)->flag == false && (*it)->opened == false && rightPressed && gWin == false && gMines > 0)
					{
						(*it)->flag = true;
						rightPressed = false;
						gMines--;

						// ���������� ���������� ������ ������� �� 1 ��� ���� ����� ������ �����
						if (yM > 0) CountFlagsAbout(-columns, 1);
						if (yM < (rows - 1)) CountFlagsAbout(columns, 1);
						if (xM > 0) CountFlagsAbout(-1, 1);
						if (xM < (columns - 1)) CountFlagsAbout(1, 1);
						if (yM > 0 && xM > 0) CountFlagsAbout(-columns - 1, 1);
						if (yM > 0 && xM < (columns - 1)) CountFlagsAbout(-columns + 1, 1);
						if (yM < (rows - 1) && xM > 0) CountFlagsAbout(columns - 1, 1);
						if (yM < (rows - 1) && xM < (columns - 1)) CountFlagsAbout(columns + 1, 1);
					}
					// ������ ����
					if ((*it)->flag && (*it)->opened == false && rightPressed && gWin == false)
					{
						(*it)->flag = false;
						rightPressed = false;
						gMines++;

						// ���������� ���������� ������ ������� �� 1 ��� ���� ����� ������ �����
						if (yM > 0) CountFlagsAbout(-columns, -1);
						if (yM < (rows - 1)) CountFlagsAbout(columns, -1);
						if (xM > 0) CountFlagsAbout(-1, -1);
						if (xM < (columns - 1)) CountFlagsAbout(1, -1);
						if (yM > 0 && xM > 0) CountFlagsAbout(-columns - 1, -1);
						if (yM > 0 && xM < (columns - 1)) CountFlagsAbout(-columns + 1, -1);
						if (yM < (rows - 1) && xM > 0) CountFlagsAbout(columns - 1, -1);
						if (yM < (rows - 1) && xM < (columns - 1)) CountFlagsAbout(columns + 1, -1);
					}
					rightPressed = false;
				}
			}
			// ����� ������ ����
			if (leftPressed)
			{
				Vector2i pixelPos = Mouse::getPosition(window); // ���������� ��������� �������

				if ((*it)->sprite.getGlobalBounds().contains(pixelPos.x, pixelPos.y))
				{
					if ((gWin == false) && ((*it)->flag == false))
					{
						if ((*it)->opened && (*it)->numberCell > 0 && (*it)->numberCell == (*it)->flagsAbout) AutoOpenCells(xM, yM, false);
						if ((*it)->isMine) gFail = true; // ���� ����, �� �������� � �������� ���� �����
						else if ((*it)->opened == false && (*it)->isMine == false)
						{ // ���� �� ����, �� ������ ����� ������ � ����� �������
							(*it)->opened = true;
							clickedVoidCells++;
						}
					}
					leftPressed = false;
				}
			}

			(*it)->DrawCells();
			window.draw((*it)->sprite);

			it++;
		}
}
//////////////////////////////////////////////////////////////////////////////
// ��������� ���� REGION END
//////////////////////////////////////////////////////////////////////////////

int ChooseMap()
{
	if (Keyboard::isKeyPressed(Keyboard::Num1))
	{
		rows = columns = 9;
		gMines = 10;
		windowHeight = 432;
		windowWidht = 432;

		NewGame(gMines, 7);

		return 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num2))
	{
		rows = columns = 16;
		gMines = 40;
		windowHeight = 432;
		windowWidht = 432;

		NewGame(gMines, 7);

		return 2;
	}
	if (Keyboard::isKeyPressed(Keyboard::Num3))
	{
		rows = 16;
		columns = 30;
		gMines = 99;
		windowHeight = 432;
		windowWidht = 600;

		NewGame(gMines, 5);

		return 3;
	}
}

void CloseCells(int i)
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

void OpenCells(int i)
{
	advance(it, i);
	if ((*it)->flag == false)
	{
		if ((*it)->opened == false)
		{
			(*it)->opened = true;
			if ((*it)->isMine) gFail = true;
			else clickedVoidCells++;
		}
	}
	advance(it, -i);
}

void AutoOpenCells(int xM, int yM, bool z)
{
	if (yM > 0) OpenCells(-columns); // �������� ������ ������
	if (yM < (rows - 1)) OpenCells(columns); // �������� ������ �����
	if (xM > 0) OpenCells(-1); // �������� ������ �����
	if (xM < (columns - 1)) OpenCells(1); // �������� ������ ������
	if (yM > 0 && xM > 0) OpenCells(-columns - 1); // �������� ������ ����� ������
	if (yM > 0 && xM < (columns - 1)) OpenCells(-columns + 1); // �������� ������ ������ ������
	if (yM < (rows - 1) && xM > 0) OpenCells(columns - 1); // �������� ������ ����� �����
	if (yM < (rows - 1) && xM < (columns - 1)) OpenCells(columns + 1); // �������� ������ ������ �����
	if (z)(*it)->used = true;
}

void CountFlagsAbout(int i, int p)
{
	advance(it, i);
	(*it)->flagsAbout += p;
	advance(it, -i);
}

void NewGame(int &mines, int chance)
{
	int minesCount = 0; // ��� �� ����
	int i = 0;
	gWin = false;
	gFail = false;
	voidCells = rows * columns - mines;
	clickedVoidCells = 0;

	CloseCells(rows * columns);

	for (int yM = 0; yM < 16; yM++)
		for (int xM = 0; xM < 30; xM++)
			Map[yM][xM] = 0;

	do
	{
		for (int yM = 0; yM < rows; yM++)
			for (int xM = 0; xM < columns; xM++)
			{
				if (Map[yM][xM] == 1) break;
				else
				{
					i = GetRandomNumber(0, chance);
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
	for (int yM = 0; yM < rows; yM++)
		for (int xM = 0; xM < columns; xM++)
		{
			// ������ ��� ������
			if (Map[yM][xM] == 0)
			{
				(*it)->isMine = false;

				if (yM > 0 && Map[yM - 1][xM] == 1) (*it)->numberCell++; // �������� ���� ������
				if (yM < (rows - 1) && Map[yM + 1][xM] == 1) (*it)->numberCell++; // �������� ���� �����
				if (xM > 0 && Map[yM][xM - 1] == 1) (*it)->numberCell++; // �������� ���� �����
				if (xM < (columns - 1) && Map[yM][xM + 1] == 1) (*it)->numberCell++; // �������� ���� ������
				if (yM > 0 && xM > 0 && Map[yM - 1][xM - 1] == 1) (*it)->numberCell++; // �������� ���� ����� ������
				if (yM > 0 && xM < (columns - 1) && Map[yM - 1][xM + 1] == 1) (*it)->numberCell++; // �������� ���� ������ ������
				if (yM < (rows - 1) && xM > 0 && Map[yM + 1][xM - 1] == 1) (*it)->numberCell++; // �������� ���� ����� �����
				if (yM < (rows - 1) && xM < (columns - 1) && Map[yM + 1][xM + 1] == 1) (*it)->numberCell++; // �������� ���� ������ �����
			}
			else
			{
				(*it)->isMine = true; count++;
			}
			it++;
		}
	it = currentCell.begin();
}

int GetRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// ������������� �����
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}
