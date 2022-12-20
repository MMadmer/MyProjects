#pragma once

struct symbolsSpace
{
	byte frontPanel[8] =
	{
		0b00000,
		0b11111,
		0b11111,
		0b11100,
		0b11100,
		0b11111,
		0b11111,
		0b00000
	};

	byte backPanel[8] =
	{
		0b00000,
		0b11111,
		0b11111,
		0b00111,
		0b00111,
		0b11111,
		0b11111,
		0b00000
	};
};
struct symbolsSpace symbols;
