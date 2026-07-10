#ifndef ASCII_WORLD_H
#define ASCII_WORLD_H


#include <iterator>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <cmath>
#include "setting.h"
#include "wallSegment.h"
#include "player.h"
#include "sprite.h"
#include "texture.h"
#include "mapCell.h"


class AsciiWorld
{
  private:
	// sf::Vector2f cameraPosition;
	std::vector < std::vector < MapCell >> mapData;

	Player player;
	  std::vector < Sprite > sprites;
	  std::vector < WallSegment > walls;


  public:

	  AsciiWorld(const std::string & mapName)
	{
		std::vector < std::string > lines;
		std::ifstream file("../res/maps/" + mapName + ".txt");

		if (!file.is_open())
		{
			std::cerr << "Не удалось открыть файл\n";
		}

		std::string line;
		while (std::getline(file, line))
		{
			lines.push_back(line);
		}

		file.close();
		size_t lineCount = lines.size();
		mapData.resize(lineCount);
		for (size_t i = 0; i < lineCount; ++i)
		{
			const char *ptr = lines[i].c_str();

			size_t pos = lines[i].find("@");
			if (pos != std::string::npos)
			{
				player.position.x = pos;
				player.position.y = i;
				lines[i][pos] = ' ';
			}
			size_t length = lines[i].length();
			mapData[i].resize(length);

			for (size_t j = 0; j < length; ++j)
				mapData[i][j] = MapCell::getType(lines[i][j]);

		}
		player.velocity.x = 3;
		player.velocity.y = 3;
		player.angle.x = 0.0f;
		player.angle.y = 0.0f;
		//player.position.z = 100.0f;
		player.lookOffset = 0.0f;

	}

	void setWallTexture(Texture * texture)
	{
		auto setWall =[&](int cellX, int cellY, int nextCellX, int nextCellY, sf::Vector2f point1,
						  sf::Vector2f point2){
			float floorHeight = mapData[cellY][cellX].floorHeight;
			float ceilingHeight = mapData[cellY][cellX].ceilingHeight;

			if (isWall(nextCellX, nextCellY, ' '))
				addWall(point1, point2, texture, floorHeight, ceilingHeight);

			MapCell nextCell = getCell(nextCellX, nextCellY);

			if (isWall(cellX, cellY, nextCell.wallType) == true)
				return;


			if ((floorHeight >= nextCell.floorHeight) || (ceilingHeight >= nextCell.ceilingHeight))
				addWall(point1, point2, texture, floorHeight, ceilingHeight);


		};
		for (size_t y = 0; y < mapData.size(); ++y)
		{
			for (size_t x = 0; x < mapData[y].size(); ++x)
			{
				if (mapData[y][x].wallType != ' ')
				{
					// float floorHeight = mapData[y][x].floorHeight;
					// float ceilingHeight = mapData[y][x].ceilingHeight;
					float fX = static_cast < float >(x) - 0.5f;
					float fY = static_cast < float >(y) - 0.5f;

					sf::Vector2f p1(fX, fY);
					sf::Vector2f p2(fX + 1, fY);
					sf::Vector2f p3(fX, fY + 1);
					sf::Vector2f p4(fX + 1, fY + 1);
					/* 
					   if (isWall(x, y - 1, ' ')) addWall(p1, p2, texture,
					   floorHeight, ceilingHeight); if (isWall(x - 1, y, ' '))
					   addWall(p1, p3, texture, floorHeight, ceilingHeight); if 
					   (isWall(x + 1, y, ' ')) addWall(p2, p4, texture,
					   floorHeight, ceilingHeight); if (isWall(x, y + 1, ' '))
					   addWall(p3, p4, texture, floorHeight, ceilingHeight); */
					setWall(x, y, x, y - 1, p1, p2);
					setWall(x, y, x - 1, y, p1, p3);
					setWall(x, y, x + 1, y, p2, p4);
					setWall(x, y, x, y + 1, p3, p4);
				}
			}
		}

	}

	void addSprite(sf::Vector2f position, char symbol, Texture * texture)
	{
		Sprite sp;
		sp.position = position;
		sp.symbol = symbol;
		sp.texture = texture;
		sprites.push_back(sp);
	}

	void spritesSort()
	{
		// const Player & player = world.getPlayer();
		for (size_t i = 0; i < sprites.size(); ++i)
		{
			float dx = sprites[i].position.x - player.position.x;
			float dy = sprites[i].position.y - player.position.y;
			sprites[i].distance = std::hypot(dx, dy);
		}

		std::sort(sprites.rbegin(), sprites.rend());
	}

	const std::vector < Sprite > &getSprites() const
	{
		return sprites;
	}

	const Sprite & getSprite(size_t i) const
	{
		return sprites[i];
	}

	void addWall(sf::Vector2f startPoint, sf::Vector2f endPoint,
				 Texture * texture, float floorHeight = 0.0, float ceilingHeight = 1.0)
	{
		WallSegment wall;
		wall.start = startPoint;
		wall.end = endPoint;
		wall.texture = texture;
		wall.floorHeight = floorHeight;
		wall.ceilingHeight = ceilingHeight;
		walls.push_back(wall);
	}

	const WallSegment & getWallSegment(size_t index) const
	{
		return walls[index];
	}

	const std::vector < WallSegment > &getWalls() const
	{
		return walls;
	}


	Player & getPlayer()
	{
		return player;
	}

	const Player & getPlayer() const
	{
		return player;
	}

	MapCell getCell(int x, int y)const
	{
		if ((y < 0) || (y >= static_cast < int >(mapData.size())))
			return MapCell(' ', 0, 0);
		if ((x < 0) || (x >= static_cast < int >(mapData[y].size())))
			return MapCell(' ', 0, 0);
		return mapData[y][x];
	}

	bool isWall(int x, int y, char symbol = '#')const
	{
		return getCell(x, y).wallType == symbol;
	}

};
	/* 
	   int main() { AsciiWorld world("map1"); } */
#endif // *ASCII_WORLD_H
