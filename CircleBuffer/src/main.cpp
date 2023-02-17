#include <SFML/Graphics.hpp>
#include "simulation.hpp"

int main()
{
	Settings simulationSettings(
		1500,
		1000,
		144,
		false,

		"Vertex Buffer Simulation"
	);

	Simulation simulation(simulationSettings);

	simulation.run();
}