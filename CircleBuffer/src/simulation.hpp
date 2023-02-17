#pragma once

#include <SFML/Graphics.hpp>
#include <string>

struct Settings
{
	const unsigned int screenWidth;
	const unsigned int screenHeight;
	const unsigned int frameRate;

	const bool vSync;

	const std::string title;
};


class Simulation : Settings
{
	sf::RenderWindow m_window;

	bool paused = false;
	bool running = true;


public:
	// constructor and destructor
	explicit Simulation(const Settings& settings) : Settings(settings)
	{
		m_window.setFramerateLimit(frameRate);
		m_window.setSize({ screenWidth, screenHeight });
		m_window.setTitle(title);
		m_window.setVerticalSyncEnabled(vSync);
	}
	~Simulation() = default;


	// function to start the simulation
	void run()
	{
		while (running)
		{
			tickSim();
		}
	}


private:
	void tickSim()
	{
		// preparing the frame
		m_window.clear();


		pollEvents();


		// finishing the frame
		m_window.display();
	}


	void pollEvents()
	{
		sf::Event event{};

		while (m_window.pollEvent(event))
		{
			if (event.type == )
		}
	}
};