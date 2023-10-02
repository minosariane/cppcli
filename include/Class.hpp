#ifndef CLASS_HPP
#define CLASS_HPP

#include <fstream>
#include <vector>

class Class
{
    public:
    Class(std::string& name);
    ~Class();
    
    void setName(std::string& name);
    std::string getName();
    void setHPP(std::string& content);
    void setCPP(std::string& content);
    void setMembers(std::vector<std::pair<std::string, std::string>>& members);
    void setMethods(std::vector<std::pair<std::string, std::string>>& methods);
    void addSettersGetters();
    void generate();
    void preview();

    private:
    void write(std::string& path, std::string& content);

    private:
    std::vector<std::pair<std::string, std::string>> members, methods;
    std::string name, HPP, CPP;
    std::fstream fileStream;
};

#endif//CLASS_HPP