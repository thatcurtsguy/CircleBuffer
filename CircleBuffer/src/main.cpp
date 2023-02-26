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
		20,
		false,

		"Vertex Buffer Simulation",

		50'000,
		6,
		6
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}
