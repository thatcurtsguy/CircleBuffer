#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../buffer/Buffer.hpp"

#include <chrono>

/*
struct Entity : Allocations
{
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

	explicit Entity(const Allocations& allocations) : Allocations(allocations) {}

	sf::Vector2f update(const sf::Vector2f mousePos)
	{
		const sf::Vector2f delta = mousePos - m_position;
		const float distSq = delta.x * delta.x + delta.y * delta.y;

		if (distSq > 0)
		{
			const float f = 20 / distSq;
			m_velocity += delta * f;
		}

		// friction
		m_velocity /= 1.1f;

		m_position += m_velocity;
		return m_velocity;
	}
};
*/

struct Settings
{
	const unsigned int screenWidth;
	const unsigned int screenHeight;
	const unsigned int frameRate;
	const unsigned int summonCount;

	const bool vSync;
	const unsigned int numThreads;

	const std::string title;

	const unsigned int maxObjects;
	const unsigned int objectPoints;
	const float objectRadius;
	const float spawnRad;
	const float initialVelocity;
	const float friction;
	const float attractionStrength;
	const float deadzone;
};


class Simulation : Settings
{
	sf::RenderWindow m_window;
	sf::Clock m_clock;

	bool m_paused = false;
	bool m_running = true;
	bool m_updateBuffer = true;

	Buffer m_buffer;
	std::vector<sf::Vector2f> m_velocities;
	std::vector<sf::Vertex>* m_vertices;

	bool mousePressed = false;
	bool mouseSide = false; // false: left, true: right

	unsigned int add_index = 0;

	
public:
	// constructor and destructor
	explicit Simulation(const Settings& settings);
	~Simulation();

	void run();

private:
	void tickSim();
	void pollEvents();
	void displayFrameRate();
	void addObject(sf::Vector2f position, bool update);
	void updatePosition(unsigned index, sf::Vector2f mousePos);
	void updatePositions();
	void objectInteraction();
	void removeObject(sf::Vector2f position, float range, bool update);
};
