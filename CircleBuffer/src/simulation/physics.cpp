#include "simulation.hpp"

#include <vector>
#include <SFML/Graphics.hpp>
#include <random>
#include <thread>



// random
inline int randint(const int start, const int end) {
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




Simulation::Simulation(const Settings& settings)
	: Settings(settings), m_window(sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), title)),
	m_buffer(maxObjects, objectPoints, sf::VertexBuffer::Stream)
{
	m_window.setFramerateLimit(frameRate);
	m_window.setVerticalSyncEnabled(vSync);

	m_vertices = m_buffer.getVertices();
	m_velocities.resize(maxObjects, sf::Vector2f());
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

	if (!m_paused)
		updatePositions();

	if (m_updateBuffer)
		m_buffer.update();

	// finishing the frame
	displayFrameRate();
	m_window.display();
}


void Simulation::addObject(const sf::Vector2f position, const bool update = true)
{
	const auto i = static_cast<float>(add_index);
	constexpr float frequency = 0.000009f;

	const auto r = static_cast<sf::Uint8>(std::sin(frequency * i + 0) * 127 + 128);
	const auto g = static_cast<sf::Uint8>(std::sin(frequency * i + 2) * 127 + 128);
	//const auto b = static_cast<sf::Uint8>(std::sin(frequency * i + 4) * 127 + 128);

	m_vertices->at(add_index).position = position;
	m_vertices->at(add_index).color = {r, g, 255};
	m_velocities.at(add_index) = randVector(-initialVelocity, initialVelocity, -initialVelocity, initialVelocity);

	add_index++;

	if (update)
		m_buffer.update();
}

void Simulation::updatePosition(const unsigned int index, const sf::Vector2f mousePos)
{
	const sf::Vector2f delta = mousePos - m_vertices->at(index).position;
	const float distSq = delta.x * delta.x + delta.y * delta.y;

	if (distSq > deadzone)
	{
		const float f = attractionStrength / distSq;
		m_velocities.at(index) += delta * f;
	}

	// friction
	m_velocities.at(index) /= friction;

	m_vertices->at(index).position += m_velocities.at(index);
}

void Simulation::updatePositions()
{
	const auto mousePos = sf::Vector2f(sf::Mouse::getPosition(m_window));

	// Split the objects into eight roughly equal parts
	const unsigned int objectsPerThread = static_cast<unsigned>(add_index) / numThreads;
	std::vector<std::thread> threads(numThreads);

	auto updateFunc = [&](const unsigned int startIndex, const unsigned int endIndex) {
		for (unsigned int i = startIndex; i < endIndex; ++i)
		{
			updatePosition(i, mousePos);
		}
	};

	// Start the threads
	for (unsigned int i = 0; i < numThreads; ++i)
	{
		unsigned int startIndex = i * objectsPerThread;
		unsigned int endIndex = (i + 1) * objectsPerThread;
		if (i == numThreads - 1)
		{
			endIndex = static_cast<int>(add_index);
		}

		threads.at(i) = std::thread(updateFunc, startIndex, endIndex);
	}

	// Wait for the threads to finish
	for (unsigned int i = 0; i < numThreads; ++i)
	{
		threads.at(i).join();
	}
}


void Simulation::objectInteraction()
{
	if (mousePressed == false)
		return;

	const auto mouse = sf::Vector2f(sf::Mouse::getPosition(m_window));

	if (mouseSide == false)
		for (unsigned int i{ 0 }; i < summonCount; i++)
		{
			const sf::Vector2f position(mouse.x + randfloat(-spawnRad, spawnRad), mouse.y + randfloat(-spawnRad, spawnRad));
			addObject(position, false);
		}

	else if (mouseSide == true)
		removeObject(sf::Vector2f(mouse), spawnRad, false);

	m_buffer.update();
}


void Simulation::removeObject(const sf::Vector2f position, const float range, const bool update=true)
{
	/*
	unsigned int index = 0;
	for (const Entity& object : m_objects)
	{
		const sf::Vector2f delta = position - object.m_position;
		const float dist = delta.x * delta.x + delta.y * delta.y;

		if (dist < (objectRadius*objectRadius + range*range))
		{
			m_buffer.remove(&object);
			m_objects.erase(m_objects.begin() + index);
		}

		else
			index++;
	}

	if (update)
		m_buffer.update();

	*/
}
