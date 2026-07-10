#ifndef WALLSEGMENT_H
#define WALLSEGMENT_H

#include <SFML/System.hpp>
#include "Texture.h"

struct WallSegment
{
	sf::Vector2f start;
	sf::Vector2f end;
	Texture* texture;
	float floorHeight;
	float ceilingHeight;
};


#endif // *WALLSEGMENT_H