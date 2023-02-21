#include "simulation.hpp"

#include <vector>
#include <SFML/Graphics.hpp>
#include <random>


// random
inline int randint(int start, int end) {
	return rand() % (end - start) + start;
}




Simulation::Simulation(const Settings& settings) : Settings(settings), m_window(sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), title)),
m_buffer(maxObjects, objectPoints)
{
	m_window.setFramerateLimit(frameRate);
	m_window.setVerticalSyncEnabled(vSync);

	m_objects.reserve(maxObjects);
}


Simulation::~Simulation() = default;

void Simulation::run()
{
	//initObjects();

	while (m_running)
	{
		tickSim();
	}
}


void Simulation::tickSim()
{
	// preparing the frame
	m_window.clear();
	pollEvents();

	// rendering
	objectInteraction();
	m_buffer.render(&m_window);

	// finishing the frame
	displayFrameRate();
	m_window.display();
}


void Simulation::initObjects()
{
	const sf::Vector2i min(55, 55);
	const sf::Vector2u spacing(100, 100);
	const sf::Vector2u amount(2, 5);

	for (unsigned int x{ 0 }; x < amount.x; x++)
	{
		for (unsigned int y{ 0 }; y < amount.y; y++)
		{
			addObject(sf::Vector2f( min.x + x * spacing.x, min.y + y * spacing.y ), true);
		}
	}
}

void Simulation::addObject(const sf::Vector2f position, const bool update = true)
{
	m_objects.emplace_back(m_buffer.add(position, objectRadius, { 55, 55, 55 }));

	if (update)
		m_buffer.update();
}


void Simulation::objectInteraction()
{
	if (mousePressed == false)
		return;

	const sf::Vector2i mouse = sf::Mouse::getPosition(m_window);
	const float r = 70;

	if (mouseSide == false)
		for (int i{ 0 }; i < 100; i++)
		{
			const sf::Vector2f position(mouse.x + randint(-r, r), mouse.y + randint(-r, r));
			addObject(position, false);
		}

	else if (mouseSide == true)
		removeObject(sf::Vector2f(mouse), r, false);

	m_buffer.update();
}


void Simulation::removeObject(const sf::Vector2f position, const float range, const bool update=true)
{
	unsigned int index = 0;
	for (const Object& object : m_objects)
	{
		const sf::Vector2f delta = position - object.position;

		if (const float dist = delta.x * delta.x + delta.y * delta.y; dist < (objectRadius*objectRadius + range*range))
		{
			m_buffer.remove(&object);
			m_objects.erase(m_objects.begin() + index);
		}

		else
			index++;
	}

	if (update)
		m_buffer.update();
}
