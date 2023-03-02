#include "simulation.hpp"
#include <SFML/Graphics.hpp>

void Simulation::pollEvents()
{
	sf::Event event{};


	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				m_running = false;
			}

			else if (event.key.code == sf::Keyboard::Space)
			{
				m_paused = not m_paused;
			}

			else if (event.key.code == sf::Keyboard::B)
			{
				m_updateBuffer = not m_updateBuffer;
			}
		}

		else if (event.type == sf::Event::MouseButtonPressed)
		{
			mousePressed = true;
			const sf::Vector2i mouse = sf::Mouse::getPosition(m_window);

			if (event.mouseButton.button == sf::Mouse::Left)
				mouseSide = false;

			else if (event.mouseButton.button == sf::Mouse::Right)
				mouseSide = true;
		}

		else if (event.type == sf::Event::MouseButtonReleased)
		{
			mousePressed = false;
		}
	}
}

void Simulation::displayFrameRate()
{
	// FPS management
	const sf::Int32 millisecondsPerFrame = m_clock.restart().asMilliseconds();

	std::ostringstream oss;
	oss << title << " " << millisecondsPerFrame << " millisecondsPerFrame" << " " << "objects: " << m_objects.size();
	const std::string stringFrameRate = oss.str();
	m_window.setTitle(stringFrameRate);
}