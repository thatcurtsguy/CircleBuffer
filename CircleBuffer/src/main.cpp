#include "simulation/simulation.hpp"
#include <ctime>

int main()
{
	// initilising random
	std::srand(static_cast<unsigned>(time(nullptr)));

	const Settings simulationSettings(
		1920,
		1000,
		400,
		4900,
		false,
		8,
		"Vertex Buffer Simulation",
		1'500'000,
		1,
		5.0f,
		0,
		10,
		1.6f,
		550,
		0
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}
