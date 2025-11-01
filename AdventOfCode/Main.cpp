
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

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
	std::fstream file("./Day19/Day19DataFormatted.txt");
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

	std::chrono::high_resolution_clock clock;
	std::vector<std::chrono::microseconds> durations;
	

	// PART 1
	Day19::Simulation::TIME_LIMIT = 25;
	durations.clear();

	std::cout << "===== " << "PART 1" << "=====" << std::endl;

	int totalQuality = 0;
	for (auto iter = simulations.begin(); iter != simulations.end(); ++iter)
	{
		Day19::Simulation& simulation = *iter;

		std::cout << "\nStarting Simulation " << simulation.GetID() << std::endl;

		auto start = clock.now();
		
		// Simulation
		int greedySolution = simulation.FindGreedySolution();
		int greedySolutionWithOre = simulation.FindGreedySolution(true);

		int bestGreedySolution = greedySolutionWithOre > greedySolution ? greedySolutionWithOre : greedySolution;
		int geodes = simulation.FindBestSolution(bestGreedySolution);

		int quality = geodes * simulation.GetID();
		totalQuality += quality;

		auto end = clock.now();

		// Time
		auto duration = end - start;
		auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		auto durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto durationSecond = std::chrono::duration_cast<std::chrono::seconds>(duration);
		auto durationMinute = std::chrono::duration_cast<std::chrono::minutes>(duration);
		durations.push_back(durationMicro);

		// Display
		std::cout << "Results for Simulation " << simulation.GetID() << "\n";
		std::cout << "\tGreedy Solution: " << greedySolution << ", With Ore: " << greedySolutionWithOre << std::endl;
		std::cout << "\tBest Solution: " << geodes << ", Quality: " << quality << std::endl;

		std::cout << "Duration - Micro: " << durationMicro.count() 
			<< " Milli: " << durationMilli.count() 
			<< " Seconds: " << durationSecond.count()
			<< " Minutes: " << durationMinute.count() << std::endl;
	}
	
	double seconds = 0;
	for (auto iter = durations.begin(); iter != durations.end(); ++iter)
	{
		auto& duration = *iter;
		seconds += duration.count() * static_cast<double>(std::chrono::seconds::period::den) / static_cast<double>(std::chrono::microseconds::period::den);
	}

	std::cout << "\nTotal Quality: " << totalQuality << std::endl;
	std::cout << "\nTotal Seconds: " << seconds << std::endl;

	// PART 2
	Day19::Simulation::TIME_LIMIT = 33;
	durations.clear();

	int BLUEPRINT_LIMIT = 3;

	std::cout << "\n\n===== " << "PART 2" << "=====" << std::endl;

	int totalGeodeMultiplier = 1;
	for (int i = 0; i < BLUEPRINT_LIMIT; i++)
	{
		Day19::Simulation& simulation = simulations[i];

		std::cout << "\nStarting Simulation " << simulation.GetID() << std::endl;

		auto start = clock.now();

		// Simulation
		int greedySolution = simulation.FindGreedySolution();
		int greedySolutionWithOre = simulation.FindGreedySolution(true);

		int bestGreedySolution = greedySolutionWithOre > greedySolution ? greedySolutionWithOre : greedySolution;
		int geodes = simulation.FindBestSolution(bestGreedySolution);

		totalGeodeMultiplier *= geodes;

		auto end = clock.now();

		// Time
		auto duration = end - start;
		auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
		auto durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		auto durationSecond = std::chrono::duration_cast<std::chrono::seconds>(duration);
		auto durationMinute = std::chrono::duration_cast<std::chrono::minutes>(duration);
		durations.push_back(durationMicro);

		// Display
		std::cout << "Results for Simulation " << simulation.GetID() << "\n";
		std::cout << "\tGreedy Solution: " << greedySolution << ", With Ore: " << greedySolutionWithOre << std::endl;
		std::cout << "\tBest Solution: " << geodes << ", Total Multiplier: " << totalGeodeMultiplier << std::endl;

		std::cout << "Duration - Micro: " << durationMicro.count()
			<< " Milli: " << durationMilli.count()
			<< " Seconds: " << durationSecond.count()
			<< " Minutes: " << durationMinute.count() << std::endl;
	}

	seconds = 0;
	for (auto iter = durations.begin(); iter != durations.end(); ++iter)
	{
		auto& duration = *iter;
		seconds += duration.count() * static_cast<double>(std::chrono::seconds::period::den) / static_cast<double>(std::chrono::microseconds::period::den);
	}

	std::cout << "\nTotal Geode Multiplier: " << totalGeodeMultiplier << std::endl;
	std::cout << "\nTotal Seconds: " << seconds << std::endl;
	

	int x;
	std::cin >> x;

	return 0;
}

/*

We are effectively traversing a graph where each node is a different possible "state".
Each state has the potential to create 4 new linked nodes, as well as advancing the current node.
Searching for the node with the largest Geode

Optimizations:

Find Greedy Solution - Do not check a solution that can not beat the Greedy Solution

Always build Geode Bot if possible

Never build more bots than neccesary to produce a bot.

Mark if Bot has been "built". Do not "rebuild" a bot at a later step.

-- Remove Bots if they have built all possible "children" or a Geode Bot
^ - ERROR - Check actually makes the code SLOWER (probably because we are already doing the "Maximum Possible Geodes Check")

Only copy the State Data (Resources and Bots)
 
Minimize the State Data aka Char > Int

Use Memoization (HashMap) to store already calculated values. 
NOTE: Memoization requires using a DFS. We are using a BFS.
NOTE: Might actually be slower...

Some Optimizations come from:
https://www.youtube.com/watch?v=5rb0vvJ7NCY 

ANSWER: 
Part 1: 1081
Part 2: 2415

*/