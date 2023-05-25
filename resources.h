#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <tuple>

struct Resources
{
    int lumber;
    int brick;
    int wool;
    int grain;
    int ore;

    Resources(void);
    Resources(int, int, int, int, int);

    int& operator[](int);
    Resources& operator-(const Resources);
    Resources& operator+(const Resources);
    Resources& operator-=(const Resources);
    Resources& operator+=(const Resources);
};