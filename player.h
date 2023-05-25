#pragma once
#include "board.h"
#include <map>

struct Player
{
    std::string name;
    int devCards;
    std::map<int, int> spacesOwned;
    std::list<Resources> resourceCombinations;

public:
    Player(std::string);

    void addCombination(Resources);
    void removeCombination(int);
    void listCombinations(void);

    int getCardsNumber(Resources);
    void removeDuplicateCombinations(void);

    void getResource(int, const int, Board);

    bool couldMakeSettlement(void);
    bool couldMakeCity(void);
    bool couldMakeRoad(void);
    bool couldBuyDevCard(void);
    bool couldDoMonopol(void);
    bool couldYearOfPlenty(void);
    bool couldThief(void);
    bool couldTrade(void);
    bool couldTradeBank(void);
};