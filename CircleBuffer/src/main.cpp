#include "simulation/simulation.hpp"
#include <ctime>
#include <random>

/*
 * TODO:
 * - random food position and generation
 * - create the snake
 * - snake movement
 * - snake -> food collision
 * - snake -> snake collision
 * - snake -> wall collision
 */

int main()
{
	// initilising random
	std::default_random_engine rng(std::random_device{}());

	const Settings simulationSettings(
		1920,
		1000,
		400,
		700,
		false,

		"Vertex Buffer Simulation",

		600'000,
		1,
		5
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}
