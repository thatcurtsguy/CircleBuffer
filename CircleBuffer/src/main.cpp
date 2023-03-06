#include "simulation/simulation.hpp"
#include <ctime>

int main()
{
	// initilising random
	std::srand(static_cast<unsigned>(time(NULL)));

	const Settings simulationSettings(
		1920,
		1000,
		200,
		30,
		false,

		"Vertex Buffer Simulation",

		100'000,
		3,
		5
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}
