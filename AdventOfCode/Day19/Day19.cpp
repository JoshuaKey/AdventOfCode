#include "Day19.h"

#include <map>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

/// SIMULATION CLASS
namespace Day19
{
	int Simulation::TIME_LIMIT = 25; // 1 -> 25 = 24 Time Steps

	void Simulation::LoadRobotCosts(std::string const& stringData)
	{
		size_t botSeperator = stringData.find(':');

		std::string id = stringData.substr(0, botSeperator);
		ID = stoi(id);

		std::string botCosts = stringData.substr(botSeperator + 1);

		LoadRobotCost(botCosts, ORE);
		LoadRobotCost(botCosts, CLAY);
		LoadRobotCost(botCosts, OBSIDIAN);
		LoadRobotCost(botCosts, GEODE);
	}

	void Simulation::LoadRobotCost(std::string & stringData, ResourceType botType)
	{
		RobotCost& botCost = m_robotCosts[botType];

		size_t botSeperator = stringData.find(';');
		size_t previousResourceSep = 0;
		size_t resourceSeperator = stringData.find(',');

		if (resourceSeperator < botSeperator)
		{
			std::string resourceNeeded = stringData.substr(previousResourceSep, resourceSeperator - previousResourceSep);
			botCost.cost[ORE] = stoi(resourceNeeded);

			previousResourceSep = resourceSeperator + 1;
			resourceSeperator = stringData.find(',', previousResourceSep);
		}
		else
		{
			std::string resourceNeeded = stringData.substr(previousResourceSep, botSeperator - previousResourceSep);
			botCost.cost[ORE] = stoi(resourceNeeded);

			stringData = stringData.substr(botSeperator + 1);
			return;
		}

		if (resourceSeperator < botSeperator)
		{
			std::string resourceNeeded = stringData.substr(previousResourceSep, resourceSeperator - previousResourceSep);
			botCost.cost[CLAY] = stoi(resourceNeeded);

			previousResourceSep = resourceSeperator + 1;
			resourceSeperator = stringData.find(',', previousResourceSep);
		}
		else
		{
			std::string resourceNeeded = stringData.substr(previousResourceSep, botSeperator - previousResourceSep);
			botCost.cost[CLAY] = stoi(resourceNeeded);

			stringData = stringData.substr(botSeperator + 1);
			return;
		}

		std::string resourceNeeded = stringData.substr(previousResourceSep, botSeperator - previousResourceSep);
		botCost.cost[OBSIDIAN] = stoi(resourceNeeded);

		stringData = stringData.substr(botSeperator + 1);
	}

	void Simulation::LoadTestRobotCosts()
	{
		// Blueprint 1
		{
			//m_robotCosts[ORE].botType = ORE;
			//m_robotCosts[ORE].cost[ORE] = 4;
			//
			//m_robotCosts[CLAY].botType = CLAY;
			//m_robotCosts[CLAY].cost[ORE] = 4;
			//
			//m_robotCosts[OBSIDIAN].botType = OBSIDIAN;
			//m_robotCosts[OBSIDIAN].cost[ORE] = 3;
			//m_robotCosts[OBSIDIAN].cost[CLAY] = 19;
			//
			//m_robotCosts[GEODE].botType = GEODE;
			//m_robotCosts[GEODE].cost[ORE] = 4;
			//m_robotCosts[GEODE].cost[OBSIDIAN] = 15;
			//// ANSWER =  0
		}
		
		// Blueprint 23
		{
			m_robotCosts[ORE].botType = ORE;
			m_robotCosts[ORE].cost[ORE] = 4;

			m_robotCosts[CLAY].botType = CLAY;
			m_robotCosts[CLAY].cost[ORE] = 3;

			m_robotCosts[OBSIDIAN].botType = OBSIDIAN;
			m_robotCosts[OBSIDIAN].cost[ORE] = 2;
			m_robotCosts[OBSIDIAN].cost[CLAY] = 7;

			m_robotCosts[GEODE].botType = GEODE;
			m_robotCosts[GEODE].cost[ORE] = 3;
			m_robotCosts[GEODE].cost[OBSIDIAN] = 8;
			// ANSWER = 11 ???
		}

		// TEST CASE 1
		{
			//m_robotCosts[ORE].botType = ORE;
			//m_robotCosts[ORE].cost[ORE] = 4;
			//
			//m_robotCosts[CLAY].botType = CLAY;
			//m_robotCosts[CLAY].cost[ORE] = 2;
			//
			//m_robotCosts[OBSIDIAN].botType = OBSIDIAN;
			//m_robotCosts[OBSIDIAN].cost[ORE] = 3;
			//m_robotCosts[OBSIDIAN].cost[CLAY] = 14;
			//
			//m_robotCosts[GEODE].botType = GEODE;
			//m_robotCosts[GEODE].cost[ORE] = 2;
			//m_robotCosts[GEODE].cost[OBSIDIAN] = 7;
			//// ANSWER = 9
		}

		// TEST CASE 2
		{
			//m_robotCosts[ORE].botType = ORE;
			//m_robotCosts[ORE].cost[ORE] = 2;
			//
			//m_robotCosts[CLAY].botType = CLAY;
			//m_robotCosts[CLAY].cost[ORE] = 3;
			//
			//m_robotCosts[OBSIDIAN].botType = OBSIDIAN;
			//m_robotCosts[OBSIDIAN].cost[ORE] = 3;
			//m_robotCosts[OBSIDIAN].cost[CLAY] = 8;
			//
			//m_robotCosts[GEODE].botType = GEODE;
			//m_robotCosts[GEODE].cost[ORE] = 3;
			//m_robotCosts[GEODE].cost[OBSIDIAN] = 12;
			//// ANSWER = 12
		}
	}

	int Simulation::FindGreedySolution(bool includeOre)
	{
		State currentState;
		currentState.stateData.bots[ORE] = 1;

		for (int i = 1; i < TIME_LIMIT; i++)
		{
			ResourceType botToBuild = INVALID;

			// Build 1 Ore Bot (if requested)
			if (currentState.stateData.bots[ORE] == 1 && includeOre)
			{
				RobotCost const& botCosts = m_robotCosts[ORE];
				if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
				{
					botToBuild = ORE;
				}
			}
			// Build Clay if possible
			else if (currentState.stateData.bots[CLAY] == 0)
			{
				RobotCost const& botCosts = m_robotCosts[CLAY];
				if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
				{
					botToBuild = CLAY;
				}
			}
			else if (currentState.stateData.bots[OBSIDIAN] == 0)
			{
				// Build Obsidian if possible
				RobotCost const& botCosts = m_robotCosts[OBSIDIAN];
				if (currentState.stateData.resources[CLAY] >= botCosts.cost[CLAY])
				{
					if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
					{
						botToBuild = OBSIDIAN;
					}
				}
				// Build Clay if we need more
				else
				{
					RobotCost const& botCosts = m_robotCosts[CLAY];
					if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
					{
						botToBuild = CLAY;
					}
				}
			}
			else
			{
				RobotCost const& botCosts = m_robotCosts[GEODE];
				// Build Geode if possible
				if (currentState.stateData.resources[OBSIDIAN] >= botCosts.cost[OBSIDIAN])
				{
					if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
					{
						botToBuild = GEODE;
					}
				}
				// Build Obsidian if need more
				else
				{
					RobotCost const& botCosts = m_robotCosts[OBSIDIAN];
					if (currentState.stateData.resources[CLAY] >= botCosts.cost[CLAY])
					{
						if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
						{
							botToBuild = OBSIDIAN;
						}
					}
					// Build Clay if need more
					else if (botCosts.cost[CLAY] < 10)
					{
						RobotCost const& botCosts = m_robotCosts[CLAY];
						if (currentState.stateData.resources[ORE] >= botCosts.cost[ORE])
						{
							botToBuild = CLAY;
						}
					}
				}
			}

			// Add Resources
			for (int resourceType = ORE; resourceType < MAX; resourceType++)
			{
				currentState.stateData.resources[resourceType] += currentState.stateData.bots[resourceType];
			}

			// Add Bots
			if (botToBuild != INVALID)
			{
				RobotCost const& botCosts = m_robotCosts[botToBuild];

				for (int resourceType = ORE; resourceType < MAX; resourceType++)
				{
					currentState.stateData.resources[resourceType] -= botCosts.cost[resourceType];
				}

				currentState.stateData.bots[botToBuild] += 1;
			}
		}

		return currentState.stateData.resources[GEODE];
	}

	int Simulation::FindBestSolution(int bestGeodesSoFar)
	{
		int maximumBots[ResourceType::MAX] = { };
		for (int botType = ORE; botType < MAX; botType++)
		{
			RobotCost const& botCost = m_robotCosts[botType];
			for (int resourceType = ORE; resourceType < MAX; resourceType++)
			{
				if (botCost.cost[resourceType] > maximumBots[resourceType])
				{
					maximumBots[resourceType] = botCost.cost[resourceType];
				}
			}
		}
		maximumBots[GEODE] = INT_MAX;

		std::vector<State> stateArray;
		stateArray.push_back(State());
		stateArray.back().stateData.bots[ORE] = 1;

		std::vector<State> newStates;

		for (int step = 1; step < TIME_LIMIT; step++)
		{
			for (auto iter = stateArray.begin(); iter != stateArray.end();)
			{
				State& currentState = *iter;

				// Check which Bots we can build
				std::vector<int> botsToBuild;
				botsToBuild.reserve(4);
				for (int botType = ORE; botType < MAX; botType++)
				{
					// Only build a maximum of 4 ORE Bots
					if (currentState.stateData.bots[botType] >= maximumBots[botType])
					{
						continue;
					}

					// Do not build Robot if we have already created a child State with that Bot
					if (currentState.builtBots[botType])
					{
						continue;
					}

					RobotCost const& botCost = m_robotCosts[botType];

					bool canBuild = true;
					for (int resourceType = ORE; resourceType < MAX; resourceType++)
					{
						if (currentState.stateData.resources[resourceType] < botCost.cost[resourceType])
						{
							canBuild = false;
						}
					}

					if (canBuild)
					{
						// IF we can build GEODE, then only build GEODE
						if (botType == GEODE)
						{
							botsToBuild.clear();
						}

						botsToBuild.push_back(botType);
					}
				}

				// Add Resources
				for (int resourceType = ORE; resourceType < MAX; resourceType++)
				{
					currentState.stateData.resources[resourceType] += currentState.stateData.bots[resourceType];
				}

				// Update Best Geodes
				if (currentState.stateData.resources[GEODE] > bestGeodesSoFar)
				{
					bestGeodesSoFar = currentState.stateData.resources[GEODE];
				}

				// Add new Bots
				for(auto botType : botsToBuild)
				{
					currentState.builtBots[botType] = true;

					State newState = { currentState.stateData, {} };

					RobotCost const& botCosts = m_robotCosts[botType];
					for (int resourceType = ORE; resourceType < MAX; resourceType++)
					{
						newState.stateData.resources[resourceType] -= botCosts.cost[resourceType];
					}
					newState.stateData.bots[botType] += 1;

					// Check if new state is actually useable
					int maximumPossibleGeodes = GetMaximumPossibleGeodes(newState, step);
					if (maximumPossibleGeodes > bestGeodesSoFar)
					{
						newStates.push_back(newState);
					}
				}

				// NOTE: This actually makes the application slower...
				//// Remove "completed" States, or states that have created as many children as they can.
				//if (currentState.builtBots[GEODE]
				//	|| (currentState.stateData.bots[OBSIDIAN] == 0 && currentState.builtBots[OBSIDIAN] && currentState.builtBots[CLAY] && currentState.builtBots[ORE])
				//	|| (currentState.stateData.bots[CLAY] == 0 && currentState.builtBots[CLAY] && currentState.builtBots[ORE])
				//	)
				//{
				//	iter = stateArray.erase(iter);
				//	continue;
				//}

				++iter;
			}

			// Add newly added bots to main State Array
			stateArray.insert(stateArray.end(), newStates.begin(), newStates.end());
			newStates.clear();
		}

		return bestGeodesSoFar;
	}

	int Simulation::GetMaximumPossibleGeodes(State const& state, int step)
	{
		// This is a quick and extremely optimistic simulation that tries to determine the 
		// maximum amount of Geodes the current state cna achieve if it builds the neccesary robot each step.

		// Used to determine the "potential" of the State and if it is worth being continued.

		if (state.stateData.bots[CLAY] == 0)
		{\
			int resource = state.stateData.resources[ORE];
			for (int i = 0; step < TIME_LIMIT; step++, i++)
			{
				if (resource >= m_robotCosts[CLAY].cost[ORE])
				{
					break;
				}

				resource += state.stateData.bots[ORE] + i;
			}
		}

		if (state.stateData.bots[OBSIDIAN] == 0)
		{
			int resource = state.stateData.resources[CLAY];
			for (int i = 0; step < TIME_LIMIT; step++, i++)
			{
				if (resource >= m_robotCosts[OBSIDIAN].cost[CLAY])
				{
					break;
				}

				resource += state.stateData.bots[CLAY] + i;
			}
		}

		if (state.stateData.bots[GEODE] == 0)
		{
			int resource = state.stateData.resources[OBSIDIAN];
			for (int i = 0; step < TIME_LIMIT; step++, i++)
			{
				if (resource >= m_robotCosts[GEODE].cost[OBSIDIAN])
				{
					break;
				}

				resource += state.stateData.bots[OBSIDIAN] + i;
			}
		}

		int remainingSteps = TIME_LIMIT - step;
		int maximumPossibleGeodes = state.stateData.resources[GEODE] + state.stateData.bots[GEODE] * remainingSteps + (remainingSteps * (remainingSteps-1) / 2);

		return maximumPossibleGeodes;
	}
}