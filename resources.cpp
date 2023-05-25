#include "resources.h"

Resources::Resources(void) : lumber(0), brick(0), wool(0), grain(0), ore(0)
{ }
Resources::Resources(int lumber, int brick, int wool, int grain, int ore)
    : lumber(lumber), brick(brick), wool(wool), grain(grain), ore(ore)
{ }

int& Resources::operator[](int index)
{
    switch (index)
    {
    case 0:
        return lumber;
    case 1:
        return brick;
    case 2:
        return wool;
    case 3:
        return grain;
    case 4:
        return ore;
    default:
        return lumber;
    }
}

Resources& Resources::operator-(const Resources res)
{
    lumber -= res.lumber;
    brick -= res.brick;
    wool -= res.wool;
    grain -= res.grain;
    ore -= res.ore;

    return *this;
}

Resources& Resources::operator+(const Resources res)
{
    lumber += res.lumber;
    brick += res.brick;
    wool += res.wool;
    grain += res.grain;
    ore += res.ore;

    return *this;
}

Resources& Resources::operator-=(const Resources res)
{
    *this = *this - res;

    return *this;
}

Resources& Resources::operator+=(const Resources res)
{
    *this = *this + res;

    return *this;
}