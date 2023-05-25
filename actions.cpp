#include "actions.h"

void makeSettlement(Player& player, std::vector<int> spaces)
{
	for (int i = 0; i < player.resourceCombinations.size(); i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;

		if (combination.brick < 1 || combination.grain < 1 || combination.lumber < 1 || combination.wool < 1)
		{
			player.removeCombination(i + 1);
			i--;
		}
		else
		{
			combination.brick--;
			combination.grain--;
			combination.lumber--;
			combination.wool--;
		}
	}

	for (auto var : spaces)
		player.spacesOwned[var]++;
}

void makeCity(Player& player, std::vector<int> spaces)
{
	for (int i = 0; i < player.resourceCombinations.size(); i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);

		Resources& combination = *it;

		if (combination.grain < 2 || combination.ore < 3)
		{
			player.removeCombination(i + 1);
			i--;
		}
		else
		{
			combination.grain -= 2;
			combination.ore -= 3;
		}
	}

	for (auto var : spaces)
		player.spacesOwned[var]++;
}

void makeRoad(Player& player)
{
	for (int i = 0; i < player.resourceCombinations.size(); i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;

		if (combination.brick < 1 || combination.lumber < 1)
		{
			player.removeCombination(i + 1);
			i--;
		}
		else
		{
			combination.brick--;
			combination.lumber--;
		}
	}
}


void buyDevCard(Player& player)
{
	player.devCards++;
	for (int i = 0; i < player.resourceCombinations.size(); i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;

		if (combination.wool < 1 || combination.grain < 1 || combination.ore < 1)
		{
			player.removeCombination(i + 1);
			i--;
		}
		else
		{
			combination.wool--;
			combination.grain--;
			combination.ore--;
		}
	}
}


void doMonopol(std::vector<Player>& players, int num, int res, std::vector<int> amount)
{
	num--;
	std::vector<int> s;
	for (int i = 0; i < players.size(); i++)
		s.push_back(players[i].resourceCombinations.size());

	int sum{};
	for (int i = 0; i < amount.size(); i++)
		sum += amount[i];
	
	int k{};
	for (int i = 0; i < players.size(); i++)
	{
		if (i == num) continue;

		for (int j = 0; j < s[i]; j++)
		{
			auto it = players[i].resourceCombinations.begin();
			std::advance(it, j);
			Resources& combination = *it;

			if (combination[res] - amount[k] != 0)
			{
				players[i].removeCombination(j + 1);
				j--;
				s[i]--;
			}
		}
		for (auto& var : players[i].resourceCombinations)
			var[res] = 0;
		k++;
	}
	
	for (auto& var : players[num].resourceCombinations)
		var[res] += sum;
}

void yearOfPlenty(Player& player)
{
	auto s = player.resourceCombinations.size();
	for (int counter = 0; counter < s; counter++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, counter);
		for (int i = 0; i < 5; i++)
			for (int j = i; j < 5; j++)
			{
				player.addCombination(*it);
				player.resourceCombinations.back()[i]++;
				player.resourceCombinations.back()[j]++;
			}
	}

	player.removeDuplicateCombinations();
}


void trade(Player& player1, Player& player2, Resources res1, Resources res2)
{
	res1 -= res2;

	auto s1 = player1.resourceCombinations.size();
	auto s2 = player2.resourceCombinations.size();

	for (int i = 0; i < s1; i++)
	{
		auto it = player1.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;
		combination -= res1;

		bool ok = true;

		for (int j = 0; j < 5; j++)
		{
			if (combination[j] < 0)
			{
				ok = false;
				break;
			}
		}
		if (ok) player1.addCombination(combination);
	}

	for (int i = 0; i < s2; i++)
	{
		auto it = player2.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;
		combination += res1;

		bool ok = true;

		for (int j = 0; j < 5; j++)
		{
			if (combination[j] < 0)
			{
				ok = false;
				break;
			}
		}
		if (ok) player2.addCombination(combination);
	}

	for (int i = 0; i < s1; i++) player1.resourceCombinations.pop_front();
	for (int i = 0; i < s2; i++) player2.resourceCombinations.pop_front();
}

void tradeBank(Player& player, Resources res1, Resources res2)
{
	res1 -= res2;

	auto s = player.resourceCombinations.size();
	
	for (int i = 0; i < s; i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;

		combination -= res1;

		bool ok = true;
		for (int j = 0; j < 5; j++)
			if (combination[j] < 0)
			{
				ok = false;
				break;
			}
		if (ok)
			player.addCombination(combination);
	}

	for (int i = 0; i < s; i++) player.resourceCombinations.pop_front();
}

void thief(Player& player1, Player& player2, Board& board, int space)
{
	//Update the thief position on the board
	for (auto& var : board.space)
		std::get<2>(var) = false;
	std::get<2>(board.space[space]) = true;

	//Creates all the new possible combinations
	auto s1 = player1.resourceCombinations.size(), s2 = player2.resourceCombinations.size();

	for (int i = 0; i < s2; i++)
	{
		auto it2 = player2.resourceCombinations.begin();
		std::advance(it2, i);
		Resources& combination2 = *it2;

		for (int counter = 0; counter < 5; counter++)
		{
			if (combination2[counter] > 0)
			{
				player2.addCombination(combination2);
				player2.resourceCombinations.back()[counter]--;

				for (int j = 0; j < s1; j++)
				{
					auto it1 = player1.resourceCombinations.begin();
					std::advance(it1, j);
					Resources& combination1 = *it1;

					player1.addCombination(combination1);
					player1.resourceCombinations.back()[counter]++;
				}
			}
		}
	}

	//Deletes the old combinations
	for (int i = 0; i < s1; i++) player1.resourceCombinations.pop_front();
	for (int i = 0; i < s2; i++) player2.resourceCombinations.pop_front();

	player1.removeDuplicateCombinations();
	player2.removeDuplicateCombinations();
}

void discard(Player& player)
{
	int num{};
	for (int i = 0; i < 5; i++)
		num += player.resourceCombinations.front()[i];
	num /= 2;

	std::vector<int> resDiscarded;
	std::cout << player.name << "'s " << num << " resources discarded (L, B, W, G, O): ";
	for (int i = 0; i < 5; i++)
	{
		resDiscarded.push_back(0);
		std::cin >> resDiscarded[i];
	}
	while (std::accumulate(resDiscarded.begin(), resDiscarded.end(), 0) != num)
	{
		std::cout << "Incorrect amount of resources discarded! Try again!\n";
		std::cout << player.name << "'s " << num << " resources discarded (L, B, W, G, O): ";
		for (int i = 0; i < 5; i++)
			std::cin >> resDiscarded[i];
	}

	auto s = player.resourceCombinations.size();
	for (int i = 0; i < s; i++)
	{
		auto it = player.resourceCombinations.begin();
		std::advance(it, i);
		Resources& combination = *it;

		combination -= Resources(resDiscarded[0], resDiscarded[1], resDiscarded[2], resDiscarded[3], resDiscarded[4]);
		auto ok = false;

		for (int j = 0; j < 5; j++)
			if (combination[j] < 0)
			{
				ok = true;
				break;
			}
		if (ok) continue;

		player.addCombination(combination);
	}

	for (int i = 0; i < s; i++) player.resourceCombinations.pop_front();
}

int dices(void)
{
	std::cout << "Dices: ";
	int diceNum; std::cin >> diceNum;
	while (diceNum < 2 || diceNum > 12)
	{
		std::cout << "Dices are impossible! Try again!";
		std::cout << "Dices: "; std::cin >> diceNum;
	}
	
	return diceNum;
}