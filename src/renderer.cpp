#ifndef RENDERER_H
#define RENDERER_H

#include <array>
#include <cmath>
#include "screen.h"
#include "asciiWorld.h"

template < std::size_t Height, std::size_t Width > class AsciiRenderer
{
  private:
	float zBuffers[Height][Width];
	int horizon;

	float fov;
  public:
	AsciiRenderer(float fov)
	{
		horizon = std::round(Height / 2.0);
		this->fov = fov;
	}

	void updateZBuffer(Screen < Height, Width > &screen)
	{
		for (size_t y = 0; y < Height; ++y)
		{
			for (size_t x = 0; x < Width; ++x)
			{
				zBuffers[y][x] = MAX_RAY_DIST;
				screen[y][x] = ' ';
			}
		}

	}

	void updateHorizonOffset(size_t screenHeight, AsciiWorld & world)
	{
		horizon = screenHeight / 2 + world.getPlayer().lookOffset;
	}

	void drawColumn(Screen < Height, Width > &screen, size_t x, int columnTop, int columnBottom,
					float correctDist, char s)
	{
		if (columnTop < 0)
		{
			columnTop = 0;
			if (columnBottom < 0)
			{
				return;
			}
		}
		if (columnBottom >= Height)
		{
			columnBottom = Height - 1;
			if (columnTop >= Height - 1)
			{
				return;
			}
		}
		if (columnBottom == columnTop)
		{
			if (zBuffers[columnTop][x] > correctDist)
			{
				screen[columnTop][x] = s;
				zBuffers[columnTop][x] = correctDist;
				return;
			}
		}

		for (int y = columnTop; y < columnBottom; ++y)
		{
			if (zBuffers[y][x] > correctDist)
			{
				screen[y][x] = s;
				zBuffers[y][x] = correctDist;
			}
		}
	}

	void drawColumn(Screen < Height, Width > &screen, size_t x, int columnTop, int columnBottom,
					float correctDist, Texture * const texture, float u)
	{
		int textureTop = columnTop;
		int textureBottom = columnBottom;

		if (columnTop < 0)
		{
			columnTop = 0;
			if (columnBottom < 0)
			{
				return;
			}
		}
		if (columnBottom >= Height)
		{
			columnBottom = Height - 1;
			if (columnTop >= Height - 1)
			{
				return;
			}
		}


		if (columnBottom == columnTop)
		{
			if (zBuffers[columnTop][x] > correctDist)
			{
				screen[columnTop][x] = (*texture)[0][0];
				zBuffers[columnTop][x] = correctDist;
				return;
			}
		}
		float width = texture->getWidth();
		float height = texture->getHeight();
		int texX = static_cast < int >(COORD_TRANSLATE(u, 0.0f, 1.0f, 0.0f, width));

		for (int y = columnTop; y < columnBottom; ++y)
		{
			int texY =
				static_cast < int >(COORD_TRANSLATE(y, textureTop, textureBottom, 0.0f, height));

			if (zBuffers[y][x] > correctDist)
			{
				screen[y][x] = (*texture)[texY][texX];
				zBuffers[y][x] = correctDist;
			}
		}
	}

	void wallRayCasting(Screen < Height, Width > &screen, AsciiWorld & world)
	{

		const Player & player = world.getPlayer();

		float startAngle = player.angle.x - fov / 2.0f;
		float angleStep = fov / static_cast < float >(Width);
		for (size_t x = 0; x < Width; ++x)
		{
			float rayAngle = startAngle + x * angleStep;
			float distance = 0.0f;
			float rayDirX = std::cos(rayAngle);
			float rayDirY = std::sin(rayAngle);


			auto walls = world.getWalls();
			for (size_t i = 0; i < walls.size(); ++i)
			{
				const WallSegment & w = walls[i];
				float wX = w.end.x - w.start.x;
				float wY = w.end.y - w.start.y;

				float vX = w.start.x - player.position.x;
				float vY = w.start.y - player.position.y;

				float det = (rayDirX * wY) - (rayDirY * wX);

				if (std::abs(det) < 0.0001f)
					continue;

				float u = (vX * rayDirY - vY * rayDirX) / det;

				float distance = (vX * wY - vY * wX) / det;
				float correctedDist = distance * std::cos(rayAngle - player.angle.x);
				int heightOffset = static_cast < int >(Height * player.position.z / correctedDist);


				// int wallHeight = std::round(Height / correctedDist);
				int wallTop =
					static_cast <
					int >(Height * w.ceilingHeight / (2.0 * correctedDist));
				int wallBottom =
					static_cast < int >(Height * w.floorHeight / (2.0 * correctedDist));



				wallTop = horizon - wallTop + heightOffset;
				wallBottom = horizon - wallBottom + heightOffset;


				if (correctedDist > 0.1f && u >= 0.0f && u <= 1.0f && correctedDist < MAX_RAY_DIST)
				{
					drawColumn(screen, x, wallTop, wallBottom, correctedDist, walls[i].texture, u);
					// drawColumn(screen, x, wallTop, wallBottom, correctedDist, '#');
				}
			}
		}
	}

	void floorCasting(Screen < Height, Width > &screen, AsciiWorld & world, Texture * floorTexture)
	{

		auto getStep =[](float distance){
			return (distance / 3.0f);
		};

		const Player & player = world.getPlayer();
		MapCell cell = world.getCell(std::round(player.position.x), std::round(player.position.y));


		float startAngle = player.angle.x - fov / 2.0f;
		float angleStep = fov / static_cast < float >(Width);


		int oldMapX = std::round(player.position.x);
		int oldMapY = std::round(player.position.y);


		for (size_t x = 0; x < Width; ++x)
		{
			float rayAngle = startAngle + x * angleStep;
			float distance = 0.0f + RAY_STEP;

			float rayDirX = std::cos(rayAngle);
			float rayDirY = std::sin(rayAngle);

			int oldWallTop = static_cast <
				int >(std::abs(2 * Height * cell.ceilingHeight / (2.0 * distance)));


			// int oldWalTop = 0
			while (distance < MAX_RAY_DIST)
			{
				int mapX = std::round(player.position.x + rayDirX * distance);
				int mapY = std::round(player.position.y + rayDirY * distance);

				MapCell cell = world.getCell(mapX, mapY);

				float correctedDist = distance * std::cos(rayAngle - player.angle.x);
				float dCorrectedDist = (distance - RAY_STEP) * std::cos(rayAngle - player.angle.x);

				float heightOffset = Height * player.position.z / correctedDist;
				float dHeightOffset = Height * player.position.z / (dCorrectedDist);

				int wallTop =
					static_cast <
					int >(Height * cell.ceilingHeight / (2.0 * correctedDist));
				wallTop = horizon - wallTop + heightOffset;

				int wallBottom = static_cast <
					int >(Height * cell.floorHeight / (2.0 * correctedDist));
				wallBottom = horizon - wallBottom + heightOffset;

				int dWallTop = static_cast <
					int >(Height * cell.ceilingHeight / (2.0 * dCorrectedDist));
				dWallTop = horizon - dWallTop + dHeightOffset;

				int dWallBottom = static_cast <
					int >(Height * cell.floorHeight / (2.0 * dCorrectedDist));
				dWallBottom = horizon - dWallBottom + dHeightOffset;


				char symbol = 'o';
				if (cell.wallType == ' ')
					symbol = '.';
				if (wallTop < dWallTop)
					drawColumn(screen, x, wallTop, dWallTop, correctedDist, symbol);
				else
					drawColumn(screen, x, dWallTop, wallTop, correctedDist, symbol);

				if (wallBottom < dWallBottom)
					drawColumn(screen, x, wallBottom, dWallBottom, correctedDist, symbol);
				else
					drawColumn(screen, x, dWallBottom, wallBottom, correctedDist, symbol);



				// distance += getStep(distance);
				distance += RAY_STEP;
				// oldWallTop = wallTop;
				// theta += dtheta;
			}


		}
	}




	void spriteRender(Screen < Height, Width > &screen, AsciiWorld & world)
	{
		const Player & player = world.getPlayer();
		world.spritesSort();
		for (size_t i = 0; i < world.getSprites().size(); ++i)
		{
			const Sprite & sprite = world.getSprites()[i];
			if (sprite.distance < 0.01f || sprite.distance > MAX_RAY_DIST)
				continue;
			float dx = sprite.position.x - player.position.x;
			float dy = sprite.position.y - player.position.y;
			float spriteAngle = std::atan2(dy, dx);
			float relativeAngle = spriteAngle - player.angle.x;
			while (relativeAngle < -PI)
				relativeAngle += 2.0f * PI;
			while (relativeAngle > PI)
				relativeAngle -= 2.0f * PI;
			if (std::abs(relativeAngle) > fov / 1.5f)
				continue;
			float correctedDist = sprite.distance * std::cos(relativeAngle);
			float heightOffset = player.position.z / correctedDist;
			float spriteCenterX =
				(Width / 2.0f) * (1.0f + std::tan(relativeAngle) / std::tan(fov / 2.0f));
			int size = std::round(Height / correctedDist);
			sf::Vector2i spriteSize =
			{
			size, size};
			int spriteTop = horizon - (spriteSize.y / 2) + heightOffset;
			int spriteBottom = horizon + (spriteSize.y / 2) + heightOffset;
			int textureTop = spriteTop;
			int textureBottom = spriteBottom;
			if (spriteTop < 0)
			{
				spriteTop = 0;
				if (spriteBottom < 0)
					spriteBottom = 0;
			}
			if (spriteBottom >= Height)
			{
				spriteBottom = Height - 1;
				if (spriteTop >= Height)
					spriteTop = Height - 1;
			}

			int spriteLeft = std::round(spriteCenterX - spriteSize.x / 2.0);
			int spriteRight = std::round(spriteCenterX + spriteSize.x / 2.0);
			int textureLeft = spriteLeft;
			int textureRight = spriteRight;
			if (spriteLeft < 0)
			{
				spriteLeft = 0;
				if (spriteRight < 0)
					continue;
			}
			if (spriteRight >= Width)
			{
				spriteRight = Width - 1;
				if (spriteLeft >= Width)
					continue;
			}

			// world.creeper.render(screen, spriteLeft,
			// spriteTop,spriteRight- 
			// spriteLeft, spriteBottom - spriteTop);
			for (int x = spriteLeft; x < spriteRight; ++x)
			{

				for (int y = spriteTop; y < spriteBottom; ++y)
				{
					if (zBuffers[y][x] < sprite.distance)
						continue;
					size_t textureY = COORD_TRANSLATE(y, textureTop, textureBottom, 0,
													  sprite.texture->getHeight());
					size_t textureX = COORD_TRANSLATE(x, textureLeft, textureRight, 0,
													  sprite.texture->getWidth());
					screen[y][x] = (*sprite.texture)[textureY][textureX];
				}
			}
		}
	}

	void renderMinimap(Screen < Height, Width > &screen, const AsciiWorld & world)
	{
		const Player & player = world.getPlayer();
		const int mapSizeX = 7;	// Радиус 
		// 
		// 
		// 
		// видимости 
		// карты
		const int mapSizeY = 5;
		for (int j = -mapSizeY; j < mapSizeY; ++j)
		{
			for (int i = -mapSizeX; i < mapSizeX; ++i)
			{
				int mapX = std::round(player.position.x + i);
				int mapY = std::round(player.position.y + j);
				// Рисуем в левый верхний
				// угол
				// экрана (смещение +
				// радиус)
				int screenX = i + mapSizeX;
				int screenY = j + mapSizeY;
				screen[screenY][screenX] = world.getCell(mapX, mapY).wallType;
			}
		}
		// Ставим маркер игрока по
		// центру
		// минимапы
		screen[mapSizeY][mapSizeX] = '@';
		for (size_t i = 0; i < world.getSprites().size(); ++i)
		{
			const Sprite & s = world.getSprite(i);
			int x = std::round(s.position.x - player.position.x + mapSizeX);
			int y = std::round(s.position.y - player.position.y + mapSizeY);
			if (0 <= x && x < 2 * mapSizeX)
			{
				if (0 <= y && y < 2 * mapSizeY)
				{
					screen[y][x] = s.symbol;
				}
			}

		}
	}

};
#endif // * RENDERER_H
