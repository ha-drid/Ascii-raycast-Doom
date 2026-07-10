#ifndef VIRTUALSTICK_H
#define VIRTUALSTICK_H

#include <SFML/Graphics.hpp>
#include <cmath>

class VirtualStick
{
  private:
	sf::Vector2f center;		// Центр базы стика
	sf::Vector2f position;		// Позиция шляпки
	float maxRadius;			// Максимальный радиус
								// движения
	float deadZone;				// Мертвая зона
	int activeFinger;			// ID активного касания
	bool isPressed;

	  sf::CircleShape baseCircle;
	  sf::CircleShape thumbCircle;

  public:
	  VirtualStick(sf::Vector2f centerPos, float baseRad, float thumbRad):center(centerPos),
		position(centerPos), maxRadius(baseRad), deadZone(15.f), activeFinger(-1), isPressed(false)
	{

		baseCircle.setRadius(baseRad);
		baseCircle.setOrigin(baseRad, baseRad);
		baseCircle.setPosition(center);
		baseCircle.setFillColor(sf::Color(100, 100, 100, 128));

		thumbCircle.setRadius(thumbRad);
		thumbCircle.setOrigin(thumbRad, thumbRad);
		thumbCircle.setPosition(center);
		thumbCircle.setFillColor(sf::Color(200, 200, 200, 200));
	}

	void handleEvent(const sf::Event & event)
	{
		if (event.type == sf::Event::TouchBegan)
		{
			sf::Vector2f touchPos(event.touch.x, event.touch.y);
			// Проверяем нажатие в области стика
			if (!isPressed
				&& std::hypot(touchPos.x - center.x, touchPos.y - center.y) <= maxRadius)
			{
				activeFinger = event.touch.finger;
				isPressed = true;
			}
		}
		else if (event.type == sf::Event::TouchMoved && isPressed)
		{
			if (event.touch.finger == activeFinger)
			{
				sf::Vector2f touchPos(event.touch.x, event.touch.y);
				sf::Vector2f offset = touchPos - center;
				float distance = std::hypot(offset.x, offset.y);

				if (distance > maxRadius)
				{
					// Ограничиваем движение шляпки
					// краем базы
					offset = (offset / distance) * maxRadius;
					position = center + offset;
				}
				else
				{
					position = touchPos;
				}
			}
		}
		else if (event.type == sf::Event::TouchEnded && isPressed)
		{
			if (event.touch.finger == activeFinger)
			{
				reset();
			}
		}
	}

	void reset()
	{
		isPressed = false;
		activeFinger = -1;
		position = center;
	}

	// Возвращает вектор направления от -1 до 1 
	// с учетом мертвой зоны
	sf::Vector2f getDirection()const
	{
		sf::Vector2f offset = position - center;
		float distance = std::hypot(offset.x, offset.y);

		if (distance < deadZone)
		{
			return sf::Vector2f(0.f, 0.f);
		}

		// Нормализуем и возвращаем вектор
		return offset / maxRadius;
	}

	void draw(sf::RenderWindow & window)
	{
		window.draw(baseCircle);
		window.draw(thumbCircle);
		thumbCircle.setPosition(position);
	}
};

#endif // VIRTUALSTICK_H
