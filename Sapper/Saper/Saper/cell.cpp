#include "cell.h"

cell::cell()
{
	tileset.loadFromFile("assets/tileset.png");
}

void cell::setCellParameters(int x, int y)
{
	if (fail) opened = true;
	sprite.setPosition(x, y);
}

void cell::drawCells()
{
	if (decorate) setTextureCell(0);
	else
	{
		//Флаг
		if (flag)
		{
			if (isMine == false && fail)
			{
				setTextureCell(2);
				if (miss == false) mines++;
				miss = true;
			}
			else setTextureCell(3);
		}
		else
		{
			//Закрытые ячейки
			if (opened == false) setTextureCell(4);
			//Мины
			if (isMine && !fail && (opened || win)) setTextureCell(0);
			//Взорванные мины
			else if (isMine && fail) setTextureCell(1);
			//Отрисовка количества мин вокруг
			else if (isMine == false && numberCell == 1 && opened) setTextureCell(6);
			else if (isMine == false && numberCell == 2 && opened) setTextureCell(7);
			else if (isMine == false && numberCell == 3 && opened) setTextureCell(8);
			else if (isMine == false && numberCell == 4 && opened) setTextureCell(9);
			else if (isMine == false && numberCell == 5 && opened) setTextureCell(10);
			else if (isMine == false && numberCell == 6 && opened) setTextureCell(11);
			else if (isMine == false && numberCell == 7 && opened) setTextureCell(12);
			else if (isMine == false && numberCell == 8 && opened) setTextureCell(13);
			else if (isMine == false && numberCell == 0 && opened) setTextureCell(5);
		}
	}
}

void cell::setTextureCell(int x)
{
	if (decorate) sprite.setScale(0.55, 0.55);
	else
	{
		if (nowNumberMap == 1) sprite.setScale(1, 1.01);
		if (nowNumberMap == 2) sprite.setScale(0.58, 0.561);
		if (nowNumberMap == 3) sprite.setScale(0.445, 0.551);
	}
	sprite.setTexture(tileset);
	sprite.setTextureRect(IntRect(x * 49, 0, 49, 49));
}
