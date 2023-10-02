#include "../include/Utils.hpp"

/**
 * @brief 
 * 
 * @param str 
 * @return std::string 
 */
std::string Utils::toUpperCase(std::string& str)
{
    std::string res = str;
    for (char& c : res) {
        c = std::toupper(c);
    }

    return res;
}

/**
* @brief 
* 
* @param str 
* @return std::string 
*/
std::string Utils::toUcFirst(std::string& str)
{
    std::string res = str;
    res[0] = std::toupper(res[0]);

    return res;
}
