#include "cell.h"

cell::cell()
{
	tileset.loadFromFile("assets/tileset.png");
}

void cell::SetCellParameters(int x, int y)
{
	if (gFail) opened = true;
	sprite.setPosition(x, y);
}

void cell::DrawCells()
{
	if (decorate) SetTextureCell(0);
	else
	{
		//Флаг
		if (flag)
		{
			if (isMine == false && gFail)
			{
				SetTextureCell(2);
				if (miss == false) gMines++;
				miss = true;
			}
			else SetTextureCell(3);
		}
		else
		{
			//Закрытые ячейки
			if (opened == false) SetTextureCell(4);
			//Мины
			if (isMine && !gFail && (opened || gWin)) SetTextureCell(0);
			//Взорванные мины
			else if (isMine && gFail) SetTextureCell(1);
			//Отрисовка количества мин вокруг
			else if (isMine == false && numberCell == 1 && opened) SetTextureCell(6);
			else if (isMine == false && numberCell == 2 && opened) SetTextureCell(7);
			else if (isMine == false && numberCell == 3 && opened) SetTextureCell(8);
			else if (isMine == false && numberCell == 4 && opened) SetTextureCell(9);
			else if (isMine == false && numberCell == 5 && opened) SetTextureCell(10);
			else if (isMine == false && numberCell == 6 && opened) SetTextureCell(11);
			else if (isMine == false && numberCell == 7 && opened) SetTextureCell(12);
			else if (isMine == false && numberCell == 8 && opened) SetTextureCell(13);
			else if (isMine == false && numberCell == 0 && opened) SetTextureCell(5);
		}
	}
}

void cell::SetTextureCell(int x)
{
	if (decorate) sprite.setScale(0.55, 0.55);
	else
	{
		if (gNowNumberMap == 1) sprite.setScale(1, 1.01);
		if (gNowNumberMap == 2) sprite.setScale(0.58, 0.561);
		if (gNowNumberMap == 3) sprite.setScale(0.445, 0.551);
	}
	sprite.setTexture(tileset);
	sprite.setTextureRect(IntRect(x * 49, 0, 49, 49));
}
