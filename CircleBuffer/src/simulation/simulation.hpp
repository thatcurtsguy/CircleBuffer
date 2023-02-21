#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "../buffer/Buffer.hpp"

#include <chrono>


struct Settings
{
	const unsigned int screenWidth;
	const unsigned int screenHeight;
	const unsigned int frameRate;

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

	Buffer m_buffer;
	std::vector<Object> m_objects;

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
	void objectInteraction();
	void removeObject(sf::Vector2f position, float range, bool update);
};
