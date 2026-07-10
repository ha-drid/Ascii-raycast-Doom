#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/System.hpp>
#include <memory>
#include "texture.h"

struct Sprite
{
	sf::Vector2f position;
	char symbol;
	float distance;
	Texture* texture;
	
	bool operator<(const Sprite& s) const
	{
		return (distance < s.distance);
	}
	bool operator>(const Sprite& s) const
	{
		return (distance > s.distance);
	}
	
	bool operator==(const Sprite& s) const
	{
		return (distance == s.distance);
	}
	
};

#endif // *SPRITE_H
