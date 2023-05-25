#include "player.h"

Player::Player(std::string name) : name(name), devCards(0)
{
    resourceCombinations.push_back(Resources());
    for (int i = 0; i < 19; i++)
        spacesOwned[i] = 0;
}

void Player::addCombination(Resources combination)
{
    resourceCombinations.push_back(combination);
}

void Player::removeCombination(int n)
{
    resourceCombinations.erase(std::next(resourceCombinations.begin(), --n));
}

void Player::listCombinations(void)
{
    std::cout << name << ":\n";
    int i{};
    for (auto& var : resourceCombinations)
    {
        std::cout << "Seed #" << ++i << "   L: " << var.lumber << " | B: " << var.brick
            << " | W: " << var.wool << " | G: " << var.grain << " | O: " << var.ore << "   " << getCardsNumber(var) << " cards\n";
    }
}

int Player::getCardsNumber(Resources combination)
{
    return (combination[0] + combination[1] + combination[2] + combination[3] + combination[4]);
}

void Player::removeDuplicateCombinations(void)
{
    for (int i = 0; i < resourceCombinations.size() - 1; i++)
    {
        auto it1 = resourceCombinations.begin();
        std::advance(it1, i);
        Resources& combination1 = *it1;
        for (int j = i + 1; j < resourceCombinations.size(); j++)
        {
            auto it2 = resourceCombinations.begin();
            std::advance(it2, j);
            Resources& combination2 = *it2;

            bool ok = true;

            for (int k = 0; k < 5; k++)
            {
                if (combination1[k] != combination2[k])
                    ok = false;
            }

            if (ok)
            {
                removeCombination(j + 1);
                j--;
            }
        }
    }
}

void Player::getResource(int space, const int power, Board board)
{
    if (spacesOwned[space] > 0 &&
        board.tellResource(space) != -1 &&
        !std::get<2>(board.space[space]))
        for (auto& var : resourceCombinations)
            var[board.tellResource(space)] += power;
}

bool Player::couldMakeSettlement(void)
{
    for (auto combination : resourceCombinations)
        if (combination[0] && combination[1] && combination[2] && combination[3])
            return true;
    return false;
}

bool Player::couldMakeCity(void)
{
    for (auto combination : resourceCombinations)
        if (combination.grain > 1 && combination.ore > 2)
            return true;
    return false;
}

bool Player::couldMakeRoad(void)
{
    for (auto combination : resourceCombinations)
        if (combination[0] && combination[1])
            return true;
    return false;
}

bool Player::couldBuyDevCard(void)
{
    for (auto combination : resourceCombinations)
        if (combination[2] && combination[3] && combination[4])
            return true;
    return false;
}

bool Player::couldDoMonopol(void)
{
    return ((devCards) ? true : false);
}

bool Player::couldYearOfPlenty(void)
{
    return ((devCards) ? true : false);
}

bool Player::couldThief(void)
{
    return ((devCards) ? true : false);
}

bool Player::couldTrade(void)
{
    return ((getCardsNumber(resourceCombinations.front())) ? true : false);
}

bool Player::couldTradeBank(void)
{
    for (auto combination : resourceCombinations)
        for (int i = 0; i < 5; i++)
            if (combination[i] > 1)
                return true;
    return false;
}