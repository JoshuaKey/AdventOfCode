#pragma once

#include <map>
#include <string>

namespace Day19
{
	constexpr int TIME_LIMIT = 25;

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

	struct Table
	{
		int resources[ResourceType::MAX] = { };
		int bots[ResourceType::MAX] = { };
		bool builtBots[ResourceType::MAX] = { };

		Table() = default;
		Table(Table const& rhs)
		{
			std::copy(rhs.resources, rhs.resources + ResourceType::MAX, resources);
			std::copy(rhs.bots, rhs.bots + ResourceType::MAX, bots);
			//std::copy(rhs.builtBots, rhs.builtBots + ResourceType::MAX, builtBots);
		}
	};

	class Simulation
	{
	public:

	public:
		void LoadTestRobotCosts();
		void LoadRobotCosts(std::string const& stringData);

	private:
		void LoadRobotCost(std::string& stringData, ResourceType botType);

	public:	
		int Simulate();

		int Do(int bestGeodesSoFar);
		int GetMaximumPossibleGeodes(Table const& state, int step);

		int FindGreedySolution(bool includeOre = false);

		int GetID() { return ID; }

	private:
		RobotCost m_robotCosts[ResourceType::MAX] = {};
		int ID = -1;
	};
}

