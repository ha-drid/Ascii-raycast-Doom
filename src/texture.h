#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include "setting.h"
#include "screen.h"



class Texture
{
  private:
	size_t height;
	size_t width;
	  std::vector < std::vector < char >>data;

  public:
	  Texture()
	{
		height = 0;
		width = 0;
	}

	Texture(const std::string & name)
	{
		width = 0;
		std::ifstream file("../res/texture/" + name + ".txt");

		if (!file.is_open())
		{
			std::cerr << "Не удалось открыть файл\n";
		}

		std::string line;

		while (std::getline(file, line))
		{
			size_t length = line.length();
			if (width < length)
				width = length;

			std::vector < char >arr;
			std::copy(line.begin(), line.end(), std::back_inserter(arr));

			data.push_back(arr);
		}

		file.close();
		height = data.size();
		for (size_t i = 0; i < data.size(); ++i)
			data[i].resize(this->width, ' ');

	}

	size_t getHeight()
	{
		return height;
	}
	
	size_t getWidth()
	{
		return width;
	}
	
	template < size_t Height, size_t Width >
		void render(Screen < Height, Width > &screen, float x_0, float y_0, float width,
					float height)
	{
		// float scale_x =
		// static_cast<float>(width)/static_cast<float>(this->max_width);

		for (float y = y_0; y < (y_0 + height); ++y)
		{
			size_t textureY = COORD_TRANSLATE(y, y_0, y_0 + height, 0, this->height);
			for (float x = x_0; x < (x_0 + width); ++x)
			{
				size_t textureX = COORD_TRANSLATE(x, x_0, x_0 + width, 0, this->width);
				int screenY = std::round(y);
				int screenX = std::round(x);
				screen[screenY][screenX] = data[textureY][textureX];
	    	}
		}
		
	}
	

	// 2. Константная версия: для объектов
	// только для чтения
	const std::vector < char >&operator[] (size_t row_index) const
	{
		return data[row_index];
	}

};


#endif // * TEXTURE_H
