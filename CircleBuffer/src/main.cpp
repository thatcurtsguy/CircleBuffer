#include "simulation/simulation.hpp"
#include <ctime>

int main()
{
	// initilising random
	std::srand(static_cast<unsigned>(time(NULL)));

	const Settings simulationSettings(
		1920,
		1000,
		400,
		700,
		false,

		"Vertex Buffer Simulation",

		500'000,
		1,
		6
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}
