#include "../include/Colour.hpp"

/**
 * @brief 
 * 
 * @param txt 
 * @return std::string 
 */
std::string Colour::Green(std::string txt)
{
    return "\033[32m" + txt + "\033[0m";
}

/**
 * @brief 
 * 
 * @param txt 
 * @return std::string 
 */
std::string Colour::Yellow(std::string txt)
{
    return "\033[33m" + txt + "\033[0m";
}
