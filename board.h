#pragma once
#include "resources.h"

struct Board
{
    std::tuple<int, std::string, bool> space[19];

public:
    Board(void);

    void initialize(void);
    void info(int);
    int tellResource(int);
};

//6 l 3 w 8 w 2 g 4 o 5 g 10 l 5 l 9 b 0 6 o 9 g 10 g 11 o 3 l 12 w 8 b 4 w 11 b