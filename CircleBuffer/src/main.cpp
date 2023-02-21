#include "simulation/simulation.hpp"
#include <ctime>

int main()
{
	// initilising random
	std::srand(static_cast<unsigned>(time(NULL)));

	const Settings simulationSettings(
		1920,
		1000,
		300,
		false,

		"Vertex Buffer Simulation",

		500'000,
		1,
		2
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}
