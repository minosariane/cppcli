#include "../include/Color.hpp"


std::string Color::Green(std::string txt)
{
    return "\033[32m" + txt + "\033[0m";
}

std::string Color::Yellow(std::string txt)
{
    return "\033[33m" + txt + "\033[0m";
}
