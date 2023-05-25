#include "board.h"

Board::Board(void)
{ }

void Board::initialize(void)
{
    for (int i = 0; i < 19; i++)
    {
        int num;
        char res;
        std::cin >> num;
        if (num == 0) space[i] = std::make_tuple(0, "Thief", true);
        else
        {
            std::cin >> res;
            switch (int(res))
            {
            case 76:
            case 108:
                space[i] = std::make_tuple(num, "Lumber", false);
                break;

            case 66:
            case 98:
                space[i] = std::make_tuple(num, "Brick", false);
                break;

            case 87:
            case 119:
                space[i] = std::make_tuple(num, "Wool", false);
                break;

            case 71:
            case 103:
                space[i] = std::make_tuple(num, "Grain", false);
                break;

            case 79:
            case 111:
                space[i] = std::make_tuple(num, "Ore", false);
                break;
            }
        }
    }
}

void Board::info(int number)
{
    std::cout << "Space " << number << " has number " << std::get<0>(space[number]) << ", is of resource " << std::get<1>(space[number])
        << " and is" << ((std::get<2>(space[number])) ? " " : " not ") << "occupied by the thief.\n";
}

int Board::tellResource(int spaceNum)
{
    if (std::get<1>(space[spaceNum]) == "Lumber") return 0;
    if (std::get<1>(space[spaceNum]) == "Brick") return 1;
    if (std::get<1>(space[spaceNum]) == "Wool") return 2;
    if (std::get<1>(space[spaceNum]) == "Grain") return 3;
    if (std::get<1>(space[spaceNum]) == "Ore") return 4;
    return -1;
}