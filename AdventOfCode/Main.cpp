
#include <iostream>
#include <fstream>
#include <vector>

#include "Day19/Day19.h"

int main()
{

	//{
	//	Day19::Simulation testSimulation;
	//	testSimulation.LoadTestRobotCosts();

	//	int geodes = testSimulation.Simulate();

	//	std::cout << "Test Simulation " << " - Geodes " << geodes << std::endl;
	//}
	

	std::vector<Day19::Simulation> simulations;

	//std::fstream file("Day19ExampleFormatted.txt");
	std::fstream file("Day19DataFormatted.txt");
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			simulations.push_back(Day19::Simulation());
			simulations.back().LoadRobotCosts(line);
		}

		file.close();
	}

	int totalQuality = 0;
	for (auto iter = simulations.begin(); iter != simulations.end(); ++iter)
	{
		Day19::Simulation& simulation = *iter;
		
		int geodes = simulation.Simulate();
		int quality = geodes * simulation.GetID();
		totalQuality += quality;

		std::cout << "Simulation " << simulation.GetID() << " - Geodes " << geodes << " (" << quality << ")" << std::endl;
	}
	std::cout << "\nTotal Quality: " << totalQuality << std::endl;

	int x;
	std::cin >> x;

	return 0;
}

/*

We are effectively traversing a graph where each node is a different possible "state".
Each state has the potential for 4 new linked nodes, in combination with its Parent Node
Searching for the node with the largest Geode

Optimizations:

Find "A" Solution

Always build Geode Bot if possible

Never build more ORE Bots than neccesary... (Max 4)

Mark if Bot has been "built". Do not "rebuild" a bot later.

If Repreatedly building Geode would give less than best Amount - Invalid Path

If Repeatedly Building Obsidian would now allow us to build 1 Geo Bot - Invalid Path

Check if Repreatedly building Clay would allow us to build 1 Obsidian Bot

Miniimize Copying


1081

*/