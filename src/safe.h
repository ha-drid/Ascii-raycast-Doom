if (sprite.distance < 0.2f || sprite.distance > MAX_RAY_DIST)
				continue;

			float dx = world.getPlayer().position.x - sprite.position.x;
			float dy = world.getPlayer().position.y - sprite.position.y;

			float spriteAngle = std::atan2(dx, dy);
			float relativeAngle = spriteAngle - world.getPlayer().angle;

			while (relativeAngle < -PI)
				relativeAngle += 2.0f * PI;
			while (relativeAngle > PI)
				relativeAngle -= 2.0f * PI;

			if (std::abs(relativeAngle) > fov / 1.5f)
				continue;

			float spriteDepth = sprite.distance * std::cos(relativeAngle);
			if (spriteDepth < 0.2f)
				continue;

			int screenX = std::round((Width / 2.0) * (1.0 + std::tan(relativeAngle) / std::tan(fov / 2.0f)));
			
			if (screenX < 0 || screenX > Width)
			continue;
			for (size_t y = 0; y < Height; ++y)
				screen[y][screenX] = sprite.symbol;
			/* 
			   float screenX = (Width / 2) * (1.0 + std::tan(relativeAngle) /
			   std::tan(fov / 2.0f));

			   float heightOffset = world.getPlayer().position.z/spriteDepth;
			   int spriteHeight = std::round(Height / spriteDepth); int
			   spriteWidth = spriteHeight; // ASCII-спрайты


			   int ceiling = horizon - (Height - spriteHeight) / 2 +
			   heightOffset; int floor = horizon + (ceiling + spriteHeight) +
			   heightOffset;

			   int startX = std::round(screenX - spriteWidth / 2.0f); int endX 
			   = std::round(screenX + spriteWidth / 2.0f);

			   // Рисуем спрайт по колонкам for (int x = 
			   startX; x < endX; ++x) { // Проверяем, что
			   колонка // находится в пределах
			   экрана if (x < 0 || x >= static_cast < int >(Width))
			   continue;

			   // --- МАГИЯ Z-БУФЕРА --- // Если
			   расстояние до спрайта // больше,
			   чем расстояние до стены // в этой
			   колонке, // значит стена скрывает
			   этот // кусок спрайта. Не рисуем
			   его! if (sprite.distance > zBuffers[x]) continue;

			   // Заполняем вертикальный столбец
			   // символом объекта for (int y = ceiling; y <
			   floor; ++y) { if (y >= 0 && y < static_cast < int >(Height)) {
			   // Здесь можно просто выводить // //
			   символ объекта screen[y][x] = sprite.symbol; } } } */
