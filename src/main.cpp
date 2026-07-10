#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Screen.h"
#include "asciiWorld.h"
#include "virtualStick.h"

#include <SFML/Graphics.hpp>
#include "screen.h"

#include "asciiWorld.h"
#include "renderer.cpp"
#include "virtualStick.h"
#include "button.h"


int main()
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(desktopMode, "ASCII 3D Engine", sf::Style::Fullscreen);
    constexpr float FOV = PI / 2.5f;	// 90 градусов

	const size_t WIDTH = 140, HEIGHT = 170;
	Screen < HEIGHT, WIDTH > screen("../res/Arial monospaced.ttf", 12,
									{
									1.f, 1.f});
	AsciiRenderer<HEIGHT, WIDTH> renderer(FOV);
	
	Texture creeper("creeper");
	
	
	Texture wall("wall");
	
	Texture test("wall");
	Texture floor("floorTexture");
	
	AsciiWorld world("map1");
	world.getPlayer().position.z = 0.75f;
	world.addSprite({5, 6}, '&', &creeper);
	world.addSprite({-7, 10}, '&', &creeper);
	world.addSprite({8, 5}, '&', &creeper);
	
	world.addWall({1, 1.6}, {4, 1.6}, &creeper);
	world.addWall({-10, -10}, {-10, 0}, &creeper);
	world.setWallTexture(&wall);
	
	
	//Texture texture("creeper");

	VirtualStick moveJoystick(sf::Vector2f(900.f, 1600.f), 120.f, 50.f);
	VirtualStick cameraJoystick(sf::Vector2f(200.f, 1600.f), 120.f, 50.f);
	Button up(
				 {
				 200, 200}, sf::Color::Green, "UP", screen.getFont());
	Button down(
				   {
				   200, 200}, sf::Color::Green, "DOWN", screen.getFont());
	up.setPosition(
					  {
					  900.0f - 100.0f, 1300.0f});
	down.setPosition(
						{
						900.0f - 100.0f, 1800});

	sf::Clock clock;
	
	
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			moveJoystick.handleEvent(event);
			cameraJoystick.handleEvent(event);
			if (up.handleEvent(event))
				world.getPlayer().position.z += dt * 1.1;
			if (down.handleEvent(event))
				world.getPlayer().position.z -= dt * 1.1;
		}

		// 1. Обновление логики (Update)
		world.getPlayer().move(moveJoystick.getDirection(), world.getWalls(), dt);
		sf::Vector2i playerPos = world.getPlayer().getMapCoord();

		MapCell playerOnCell = world.getCell(playerPos.x, playerPos.y);
		world.getPlayer().vertMove(false, playerOnCell, dt);
		world.getPlayer().cameraRotate(cameraJoystick.getDirection(), WIDTH, FOV, dt);

		// 2. Отрисовка (Render)
		window.clear(sf::Color::Black);
		//whera
		renderer.updateZBuffer(screen);
		renderer.updateHorizonOffset(HEIGHT, world);
		
		//renderer.wallRayCasting(screen, world);
		//renderer.floorCasting(screen, world, &floor);
		
		
		renderer.wallRayCasting(screen, world);
		renderer.floorCasting(screen, world, &floor);
		
		//renderer.spriteRender(screen, world);
		renderer.renderMinimap(screen, world);	// Поверх
		//texture.render(screen, 20, 40, 40, 60);
		
		
		// Вывод на экран телефона
		screen.draw(window);
		cameraJoystick.draw(window);
		moveJoystick.draw(window);
		up.draw(window);
		down.draw(window);
		window.display();
	}
	return 0;
}
