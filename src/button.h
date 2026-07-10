#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <vector>


class Button
{
  public:
	Button(sf::Vector2f size, sf::Color color, const std::string & textValue, sf::Font & font)
	{
		// Прямоугольник по умолчанию
		shape.setPointCount(4);
		shape.setPoint(0, sf::Vector2f(0, 0));
		shape.setPoint(1, sf::Vector2f(size.x, 0));
		shape.setPoint(2, sf::Vector2f(size.x, size.y));
		shape.setPoint(3, sf::Vector2f(0, size.y));

		shape.setFillColor(color);

		// Настройка текста
		text.setFont(font);
		text.setString(textValue);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);

		centerText();
		isPressed = false;
	}

	void setCustomShape(const std::vector < sf::Vector2f > &points)
	{
		shape.setPointCount(points.size());
		for (size_t i = 0; i < points.size(); ++i)
		{
			shape.setPoint(i, points[i]);
		}
		centerText();
	}

	void setPosition(sf::Vector2f pos)
	{
		shape.setPosition(pos);
		centerText();
	}

	void draw(sf::RenderWindow & window)
	{
		window.draw(shape);
		window.draw(text);
	}

	// Тот самый метод, который теперь сам
	// обрабатывает Event
	bool handleEvent(const sf::Event & event)
	{

		if (event.type == sf::Event::TouchBegan || isPressed)
		{
			sf::Vector2f touchPos(event.touch.x, event.touch.y);
			// Проверяем нажатие в области
			// фигуры
			sf::FloatRect bounds = shape.getGlobalBounds();

			if (bounds.contains(static_cast < sf::Vector2f > (touchPos)))
			{
				isPressed = true;
			}
			else
				isPressed = false;
		}
		else if (event.type == sf::Event::TouchEnded && isPressed)
		{
			isPressed = false;
		}

		if (isPressed)
		{
			sf::Vector2f touchPos(event.touch.x, event.touch.y);
			// Проверяем нажатие в области
			// фигуры
			sf::FloatRect bounds = shape.getGlobalBounds();

			if (bounds.contains(static_cast < sf::Vector2f > (touchPos)))
			{
				isPressed = true;
			}
			else
				isPressed == false;
		}
		return isPressed;

	}

  private:
	sf::ConvexShape shape;
	sf::Text text;
	bool isPressed;

	void centerText()
	{
		sf::FloatRect shapeBounds = shape.getLocalBounds();
		sf::FloatRect textRect = text.getLocalBounds();

		text.setOrigin(textRect.left + textRect.width / 2.0f,
					   textRect.top + textRect.height / 2.0f);

		text.setPosition(shape.getPosition().x + shapeBounds.width / 2.0f,
						 shape.getPosition().y + shapeBounds.height / 2.0f);
	}
};


#endif // *BUTTON_H
