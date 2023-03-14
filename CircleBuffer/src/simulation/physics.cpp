#include "simulation.hpp"

#include <vector>
#include <SFML/Graphics.hpp>
#include <random>


// random
inline int randint(int start, int end) {
	return rand() % (end - start) + start;
}

inline float randfloat(const float start, const float end)
{
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (end - start)) + start;
}

inline sf::Vector2f randVector(const float start1, const float end1, const float start2, const float end2)
{
	return { randfloat(start1, end1), randfloat(start2, end2) };
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
	updatePositions();

	if (m_updateBuffer)
		m_buffer.update();

	// finishing the frame
	displayFrameRate();
	m_window.display();
}


void Simulation::addObject(const sf::Vector2f position, const bool update = true)
{
	m_objects.emplace_back(m_buffer.add(position, objectRadius, { 55, 55, 55 }));
	m_objects.back().m_velocity = randVector(-3, 3, -3, 3);

	const unsigned int i = m_objects.back().indexes[0];
	constexpr float frequency = 0.00003f;

	const unsigned int r = static_cast<int>(std::sin(frequency * i + 0) * 127 + 128);
	const unsigned int g = static_cast<int>(std::sin(frequency * i + 2) * 127 + 128);
	const unsigned int b = static_cast<int>(std::sin(frequency * i + 4) * 127 + 128);

	m_objects.back().setColor(*m_buffer.getVertices(), sf::Color( r, g, 255 ));

	if (update)
		m_buffer.update();
}


void Simulation::updatePositions()
{
	const auto mousePos = sf::Vector2f(sf::Mouse::getPosition(m_window));
	for (Entity& object : m_objects)
	{
		object.update(*m_buffer.getVertices(), mousePos);
		const float f = 0.03f;
		object.m_velocity += randVector(-f, f, -f, f);
	}
}


void Simulation::objectInteraction()
{
	if (mousePressed == false)
		return;

	const sf::Vector2i mouse = sf::Mouse::getPosition(m_window);
	constexpr float r = 130;

	if (mouseSide == false)
		for (int i{ 0 }; i < summonCount; i++)
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
