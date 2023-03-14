#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../buffer/Buffer.hpp"

#include <chrono>


struct Entity : Object
{
	sf::Vector2f m_velocity;

	explicit Entity(const Object& object) : Object(object)
	{

	}

	void update(std::vector<sf::Vertex>& vertices, const sf::Vector2f mousePos)
	{
		const sf::Vector2f delta = mousePos - position;
		const float distSq = delta.x * delta.x + delta.y * delta.y;
		const float f = 20 / distSq;
		m_velocity += (mousePos - position) * f;

		setPosition(vertices, position + m_velocity);

		m_velocity /= 1.1f;
	}

};


struct Settings
{
	const unsigned int screenWidth;
	const unsigned int screenHeight;
	const unsigned int frameRate;
	const unsigned int summonCount;

	const bool vSync;

	const std::string title;

	const unsigned int maxObjects;
	const unsigned int objectPoints;
	const double objectRadius;
};


class Simulation : Settings
{
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	bool m_paused = false;
	bool m_running = true;
	bool m_updateBuffer = true;

	Buffer m_buffer;
	std::vector<Entity> m_objects;

	bool mousePressed = false;
	bool mouseSide; // false: left, true: right


public:
	// constructor and destructor
	explicit Simulation(const Settings& settings);
	~Simulation();

	void run();

private:
	void tickSim();
	void pollEvents();
	void initObjects();
	void displayFrameRate();
	void addObject(sf::Vector2f position, bool update);
	void updatePositions();
	void objectInteraction();
	void removeObject(sf::Vector2f position, float range, bool update);
};
