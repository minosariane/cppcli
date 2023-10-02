#include "../include/Class.hpp"
#include "../include/Utils.hpp"

#include <iostream>
#include <regex>

/**
 * @brief Construct a new Class:: Class object
 * 
 * @param name 
 */
Class::Class(std::string& name)
{
    this->setName(name);
    this->HPP =  "#ifndef " + Utils::toUpperCase(this->name) + "_HPP\n";
    this->HPP += "#define " + Utils::toUpperCase(this->name) + "_HPP\n\n";
    this->HPP += "class " + this->name + "\n";
    this->HPP += "{\n";
    this->HPP += "\tpublic:\n\t" + this->name + "();\n\t~" + this->name + "();\n\n";
    this->HPP += "\tprivate:\n";
    this->HPP += "};\n\n";
    this->HPP += "#endif//" + Utils::toUpperCase(this->name) + "_HPP";

    this->CPP =  "#include \"../include/" + this->name + ".hpp\"\n\n";
    this->CPP += this->name + "::" + this->name + "()\n";
    this->CPP += "{\n\n}\n\n";
    this->CPP += this->name + "::~" + this->name + "()\n";
    this->CPP += "{\n\n}\n\n";
}

/**
 * @brief Destroy the Class:: Class object
 * 
 */
Class::~Class()
{

}

/**
 * @brief 
 * 
 * @param name 
 */
void Class::setName(std::string& name)
{
    this->name = Utils::toUcFirst(name);
}

/**
 * @brief 
 * 
 * @return std::string 
 */
std::string Class::getName()
{
    return this->name;
}

/**
 * @brief 
 * 
 * @param content 
 */
void Class::setHPP(std::string& content)
{
    this->HPP = content;
}

/**
 * @brief 
 * 
 * @param content 
 */
void Class::setCPP(std::string& content)
{
    this->CPP = content;
}

/**
 * @brief 
 * 
 * @param members 
 */
void Class::setMembers(std::vector<std::pair<std::string, std::string>>& members)
{
    this->members = members;
}

/**
 * @brief 
 * 
 * @param methods 
 */
void Class::setMethods(std::vector<std::pair<std::string, std::string>>& methods)
{
    this->methods = methods;
}

/**
 * @brief 
 * 
 */
void Class::addSettersGetters()
{
    std::regex regex(R"((\n\tprivate\:))");
    for (auto& member : this->members) {
        this->CPP += "void " + this->name + "::set" + Utils::toUcFirst(member.second) + "(" + member.first + "& " + member.second + ")\n";
        this->CPP += "{\n";
        this->CPP += "\tthis->" + member.second + " = " + member.second + ";\n}\n\n";
        this->CPP += member.first + " " + this->name + "::get" + Utils::toUcFirst(member.second) + "()\n";
        this->CPP += "{\n";
        this->CPP += "\treturn this->" + member.second + ";\n}\n\n";
        std::string rempacement("\t" + member.first + " get" + Utils::toUcFirst(member.second) + "();\n$1");
        this->HPP = std::regex_replace(this->HPP, regex, rempacement);
        rempacement = "\tvoid set" + Utils::toUcFirst(member.second) + "(" + member.first + "& " + member.second + ");\n$1";
        this->HPP = std::regex_replace(this->HPP, regex, rempacement);
    }
}

/**
 * @brief 
 * 
 */
void Class::generate()
{
    std::regex regex(R"((\n};))");
        
    for (auto& member : this->members) {
        std::string rempacement("\n\t" + member.first + " " + member.second + ";$1");
        this->HPP = std::regex_replace(this->HPP, regex, rempacement);
    }

    regex = R"((\n\tprivate\:))";
    for (auto& method : this->methods) {
        std::string rempacement("\t" + method.first + " " + method.second + "();\n$1");
        this->HPP = std::regex_replace(this->HPP, regex, rempacement);
        this->CPP += method.first + " " + this->name + "::" + method.second + "()\n";
        this->CPP += "{\n\n}\n\n";
    }

    std::string path = "include/" + this->name + ".hpp";
    this->write(path, this->HPP);
    path = "src/" + this->name + ".cpp";
    this->write(path, this->CPP);   
}

/**
 * @brief 
 * 
 */
void Class::preview()
{
    std::cout << "\n/// HPP\n\n" << this->HPP << "\n\n/// CPP\n\n" << this->CPP << std::endl;
}

// PRIVATE

/**
 * @brief 
 * 
 * @param path 
 * @param content 
 */
void Class::write(std::string& path, std::string& content)
{
    this->fileStream.open(path, std::ios::out);
    if (!this->fileStream.is_open()) {
        std::cerr << "Cannot open file" << std::endl;
        exit(1);
    }
    this->fileStream << content;
    this->fileStream.close();
}