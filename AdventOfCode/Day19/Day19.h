#pragma once

#include <map>
#include <string>
#include <unordered_map>

namespace Day19
{
	enum ResourceType
	{
		INVALID = -1,
		ORE,
		CLAY,
		OBSIDIAN,
		GEODE,
		MAX
	};

	struct RobotCost
	{
		ResourceType botType = ORE;
		int cost[ResourceType::MAX] = { };
	};

	struct StateData
	{
		char resources[ResourceType::MAX] = { };
		char bots[ResourceType::MAX] = { };
	};

	struct State
	{
		StateData stateData = {};
		bool builtBots[ResourceType::MAX] = { };
	};

	class Simulation
	{
	public:

		static int TIME_LIMIT;

	public:
		void LoadTestRobotCosts();
		void LoadRobotCosts(std::string const& stringData);

	private:
		void LoadRobotCost(std::string& stringData, ResourceType botType);

	public:	
		int FindGreedySolution(bool includeOre = false);
		int FindBestSolution(int bestGeodesSoFar);
		int GetMaximumPossibleGeodes(State const& state, int step);

	public:
		int GetID() { return ID; }

	private:
		RobotCost m_robotCosts[ResourceType::MAX] = {};
		int ID = -1;
	};
}

