#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "globals.h"

using namespace sf;

class cell
{
public:
	Texture tileset;
	Sprite sprite;
	bool opened = false;
	bool isMine = false;
	bool flag = false;
	bool used = false;
	int flagsAbout = 0;
	int numberCell = 0;
	bool decorate = false;
	int miss = false;

	cell();

	void SetCellParameters(int x, int y);

	void DrawCells();

	void SetTextureCell(int x);
};
