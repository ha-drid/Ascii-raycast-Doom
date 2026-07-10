#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

// Объявляем шаблонный класс с двумя
// параметрами типа size_t (или unsigned int)
template < std::size_t Height, std::size_t Width > class Screen
{
  private:
	// Вместо sf::Vector2u теперь используем
	// параметры шаблона
	// Они известны на этапе компиляции,
	// поэтому size можно сделать константой
	const sf::Vector2u size
	{
	static_cast < unsigned int >(Height), static_cast < unsigned int >(Width)};
	sf::Vector2u cellSize =
	{
	20, 40};
	sf::Font font;
	unsigned int fontSize;
	sf::Vector2f scale;
	sf::Vector2f position;
	sf::Vector2f symbolSize;
	// Здесь будет ваш двумерный массив
	// символов (например, для текстовой
	// консоли)
	// Так как размеры известны заранее,
	// можно использовать фиксированный
	// массив:
	char grid[Height][Width];

  public:
	// Конструктор теперь принимает только
	// путь к шрифту
	Screen(const std::string & fontPath, unsigned fontSize, sf::Vector2f scale)
	{
		if (font.loadFromFile(fontPath) == 0)
		{
			std::cerr << "Не удалось загрузить шрифт: " << fontPath <<
				std::endl;
		}
		// Вызывать строго после font.loadFromFile(...)
		const_cast < sf::Texture & >(font.getTexture(fontSize)).setSmooth(true);


		this->fontSize = fontSize;
		this->scale = scale;
		this->position = sf::Vector2f(0, 0);


		float cellWidth = font.getGlyph('@', fontSize, false).advance * scale.x;
		float cellHeight = fontSize * scale.y;
		this->symbolSize = sf::Vector2f(cellWidth, cellHeight);
		// Пример инициализации сетки
		// (заполняем пробелами)
	}

	
	char *operator[] (std::size_t y)
	{
		return grid[y];
	}

	// 2. Для чтения из константных объектов
	// (const Screen):
	const char *operator[] (std::size_t y) const
	{
		return grid[y];
	}

	void setPosition(float x, float y)
	{
		this->position = sf::Vector2f(x, y);
	}

	float getSymbolWidth()
	{
		return symbolSize.x;
	}

	float getSymbolHeight()
	{
		return symbolSize.y;
	}
	// Геттеры для проверки работы шаблона
	unsigned int getWidth() const
	{
		return Width;
	}
	unsigned int getHeight() const
	{
		return Height;
	}
	
	sf::Font& getFont()
	{
		return font;
	}
	
	const sf::Font& getFont() const
	{
		return font;
	}


	void draw(sf::RenderWindow & window)
	{
		// Создаем ОДИН объект текста для
		// отрисовки (не пересоздаем текстуры)
		sf::Text textObject;
		textObject.setFont(font);


		textObject.setCharacterSize(fontSize);
		textObject.setScale(scale);
		textObject.setFillColor(sf::Color::White);

		// Получаем метрики символа '@' для
		// расчета размера ячейки
		// advance — это расстояние, на которое
		// сдвигается курсор после печати
		// символа

		for (std::size_t y = 0; y < Height; ++y)
		{
			for (std::size_t x = 0; x < Width; ++x)
			{
				textObject.setString(sf::String(grid[y][x]));
				textObject.setPosition(position.x + x * symbolSize.x,
									   position.y + y * symbolSize.y);
				window.draw(textObject);
				// grid[y][x] = '.'; } */
			}
		}

	}


};

#endif // SCREEN_H
