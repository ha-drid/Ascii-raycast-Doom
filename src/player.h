#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/System.hpp>
#include <cmath>
#include "setting.h"
#include "wallSegment.h"
#include "mapCell.h"

struct Player
{
	sf::Vector3f position;
	sf::Vector2f velocity;
	float velocityZ = 0.0f;
	float radius = 0.3f;
	float height = 0.75f;
	  sf::Vector2f angle;
	float lookOffset;

	/* 
	   void updatePhysics(const std::vector < WallSegment > &walls, AsciiWorld 
	   & world, float dt) {

	   // ========================================== // 1.
	   ВЕРТИКАЛЬНАЯФИЗИКА(ГРАВИТАЦИЯИПОЛ) //
	   ========================================== const float GRAVITY // =
	   15.0f; // Сила притяжения

	   // Применяем гравитацию к вертикальной
	   скоростиplayer.velocityZ -= GRAVITY * dt; //
	   Применяемвертикальнуюскоростькпозицииplayer.z 
	   += player.velocityZ * dt;

	   // Узнаем высоту пола под игроком в
	   текущейклеткеint currentMapX =
	   std::round(player.position.x); int currentMapY =
	   std::round(player.position.y); MapCell currentCell =
	   world.getCell(currentMapX, currentMapY);

	   // Коллизия с полом if (player.z <=
	   currentCell.floorHeight) { player.z = currentCell.floorHeight; // Не
	   даемпровалитьсяplayer.velocityZ = 0.0f; //
	   Обнуляемскоростьпадения // Здесь можно 
	   добавить логику // прыжка: // if
	   (jumpButtonPressed) player.velocityZ = 5.0f; }

	   // ========================================== // 2.
	   ГОРИЗОНТАЛЬНОЕДВИЖЕНИЕ // == == == == == == == == 
	   == == == == == == == == == == == == == // Применяем
	   управлениеджойстикомplayer.position.x +=
	   player.velocity.x * dt; player.position.y += player.velocity.y * dt;

	   // ========================================== // 3. КОЛЛИЗИИ
	   СОСТЕНАМИ(Идеальноескольжение) // == == ==
	   == == == == == == == == == == == == == == == == == == // Мы // //
	   делаем // 3 итерации(цикла),
	   чтобыперсонажнезастревал // в // //
	   углах, // где // стыкуются
	   несколькостен.for (int iter = 0; iter < 3; ++iter) {

	   for (const auto & wall:walls) { // Вектор самой стены
	   (отA � ���оB) float wallDx = wall.end.x - wall.start.x;
	   float wallDy = wall.end.y - wall.start.y;

	   // Вектор от начала стены до центра
	   игрокаfloat pDx = player.position.x - wall.start.x; float pDy =
	   player.position.y - wall.start.y;

	   // Квадрат длины стены float wallLenSq = // wallDx *
	   wallDx + wallDy * wallDy; if (wallLenSq == 0.0f) continue;

	   // Скалярное произведение для // поиска
	   ближайшейточкинапрямойfloat t = (pDx * wallDx +
	   pDy * wallDy) / wallLenSq;

	   // Ограничиваем t от 0 до 1, чтобы // точка
	   находиласьстрогонаОТРЕЗКЕстеныt =
	   std::max(0.0f, std::min(1.0f, t));

	   // Координаты этой ближайшей точки // //
	   на стенеfloat closestX = wall.start.x + t * wallDx; float
	   closestY = wall.start.y + t * wallDy;

	   // Вектор от ближайшей точки до // игрока
	   float distX = player.position.x - closestX; float distY =
	   player.position.y - closestY; float distanceSq = distX * distX + distY * 
	   distY;

	   // Если расстояние меньше радиуса
	   игрока—мыврезались!if (distanceSq > 0 && distanceSq <
	   player.radius * player.radius) { float distance = std::sqrt(distanceSq);

	   // Насколько глубоко мы // провалились в
	   стену? float penetration = player.radius - distance;

	   // Нормализуем вектор // выталкивания float
	   pushX = distX / distance; float pushY = distY / distance;

	   // ВЫТАЛКИВАЕМ игрока из стены. // // // Это
	   автоматическизаставитегоплавноскользитьвдольнеё!player.position.
	   x += pushX * penetration; player.position.y += pushY * penetration; } }}} */
	  sf::Vector2i getMapCoord()
	{
		int mapX = static_cast < int >(std::round(position.x));
		int mapY = static_cast < int >(std::round(position.y));
		  return sf::Vector2i(mapX, mapY);
	}

	void vertMove(bool isJumpPressed, MapCell playerOnCell, float dt)
	{
		velocityZ -= GRAVITY * dt;
		position.z += velocityZ * dt;
		static float dH = 0.05f;
		if ((position.z - height) < (playerOnCell.floorHeight + dH))
		{
			position.z = playerOnCell.floorHeight + height + dH;
			velocityZ = 0;
		}
		else if ((position.z - height) < (playerOnCell.ceilingHeight + dH))
		{
			position.z = playerOnCell.ceilingHeight + height + dH;
			velocityZ = 0;
		}
		// int mapX = static_cast<int>(std::round(position.x));
		// int mapY = static_cast<int>(std::round(position.y));
	}




	void move(sf::Vector2f joystickDirection, const std::vector < WallSegment > &walls, float dt)
	{
		float x = joystickDirection.x * velocity.x * dt;
		float y = joystickDirection.y * velocity.y * dt;
		float cosA = std::cos(angle.x + PI / 2);
		float sinA = std::sin(angle.x + PI / 2);
		float moveX = x * cosA - y * sinA;
		float moveY = x * sinA + y * cosA;
		position.x += moveX;
		position.y += moveY;

		float playerBottom = position.z - height;
		for (int iter = 0; iter < 3; ++iter)
		{
		  for (const auto & wall:walls)
			{
				// Вектор самой стены (от A до B)
				float wallDx = wall.end.x - wall.start.x;
				float wallDy = wall.end.y - wall.start.y;
				float wallTop = wall.ceilingHeight;
				float wallBottom = wall.floorHeight;
				if ((wallBottom <= playerBottom && wallTop >= playerBottom) ||
					(wallBottom <= position.z && wallTop >= position.z))
				{

					// Вектор от начала стены до
					// центра 
					// 
					// 
					// игрока
					float pDx = position.x - wall.start.x;
					float pDy = position.y - wall.start.y;
					// Квадрат длины стены
					float wallLenSq = wallDx * wallDx + wallDy * wallDy;
					if (wallLenSq == 0.0f)
						continue;
					// Скалярное произведение для
					// поиска ближайшей точки на
					// прямой
					float t = (pDx * wallDx + pDy * wallDy) / wallLenSq;
					// Ограничиваем t от 0 до 1, чтобы
					// точка находилась строго на
					// ОТРЕЗКЕ стены
					t = std::max(0.0f, std::min(1.0f, t));
					// Координаты этой ближайшей
					// точки 
					// 
					// 
					// на стене
					float closestX = wall.start.x + t * wallDx;
					float closestY = wall.start.y + t * wallDy;
					// Вектор от ближайшей точки до
					// игрока
					float distX = position.x - closestX;
					float distY = position.y - closestY;
					float distanceSq = distX * distX + distY * distY;
					// Если расстояние меньше
					// радиуса
					// игрока — мы врезались!
					if (distanceSq > 0 && distanceSq < radius * radius)
					{
						float distance = std::sqrt(distanceSq);
						// Насколько глубоко мы
						// провалились в стену?
						float penetration = radius - distance;
						// Нормализуем вектор
						// выталкивания
						float pushX = distX / distance;
						float pushY = distY / distance;
						// ВЫТАЛКИВАЕМ игрока из
						// стены. 
						// Это автоматически заставит
						// его плавно скользить вдоль
						// неё!
						position.x += pushX * penetration;
						position.y += pushY * penetration;
					}
				}
			}
		}
	}
	void cameraRotate(sf::Vector2f direction, int width, float fov, float dt)
	{
		float speed = 0.4;
		angle.x += direction.x * dt;
		angle.y -= direction.y * speed * dt;
		if (angle.y > 1.4f)
			angle.y = 1.4f;
		else if (angle.y < -1.4f)
			angle.y = -1.4f;
		float f = static_cast < float >(width) / (2.0f * std::tan(fov / 2.0f));
		lookOffset = static_cast < int >(f * std::tan(angle.y));
	}
};
#endif // *PLAYER_H
