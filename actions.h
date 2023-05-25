#pragma once
#include "player.h"
#include <iterator>
#include <numeric>

void makeSettlement(Player&, std::vector<int>);
void makeCity(Player&, std::vector<int>);
void makeRoad(Player&);

void buyDevCard(Player&);

void doMonopol(std::vector<Player>&, int, int, std::vector<int>);
void yearOfPlenty(Player&);
void thief(Player&, Player&, Board&, int);

void trade(Player&, Player&, Resources, Resources);
void tradeBank(Player&, Resources, Resources);

void discard(Player&);

int dices(void);