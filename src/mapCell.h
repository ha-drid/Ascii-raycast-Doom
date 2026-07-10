#ifndef MAPCELL_H
#define MAPCELL_H

#include <cassert>

struct MapCell
{
	char wallType;				// '#' или ' ' или 'A'
	float floorHeight;			// Высота пола в этой
	// клетке (например, 0.0 —
	// норма, 1.0 — подиум)
	float ceilingHeight;		// Высота потолка
	// (например, 3.0)

	  MapCell(char wallType, float floorHeight, float ceilingHeight)
	{
		this->wallType = wallType;

		this->floorHeight = floorHeight;
		this->ceilingHeight = ceilingHeight;
		assert(floorHeight <= ceilingHeight);
	}
	MapCell() = default;

	static MapCell getType(char symbol)
	{
		MapCell result;
		switch (symbol)
		{
		case '#':
			result = MapCell('#', 0, 2.0);
			break;

		case 'x':
			result = MapCell('x', 0, 1);
			break;

		case 'X':
			result = MapCell('X', -2.0, 5.3);
			break;


		default:
			result = MapCell(' ', 0, 0);
		}
		return result;
	}
};



#endif // *MAPCELL_H
