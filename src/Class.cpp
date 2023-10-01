#include "../include/Class.hpp"
#include <iostream>
#include <regex>

Class::Class(std::string& name)
{
    this->setName(name);
    this->HPP =  "#ifndef " + this->toUpperCase(this->name) + "_HPP\n";
    this->HPP += "#define " + this->toUpperCase(this->name) + "_HPP\n\n";
    this->HPP += "class " + this->name + "\n";
    this->HPP += "{\n";
    this->HPP += "\tpublic:\n\t" + this->name + "();\n\t~" + this->name + "();\n\n";
    this->HPP += "\tprivate:\n";
    this->HPP += "};\n\n";
    this->HPP += "#endif//" + this->toUpperCase(this->name) + "_HPP";

    this->CPP =  "#include \"../include/" + this->name + ".hpp\"\n\n";
    this->CPP += this->name + "::" + this->name + "()\n";
    this->CPP += "{\n\n}\n\n";
    this->CPP += this->name + "::~" + this->name + "()\n";
    this->CPP += "{\n\n}\n\n";
}

Class::~Class()
{

}

void Class::setName(std::string& name)
{
    this->name = toUcFirst(name);
}

std::string Class::getName()
{
    return this->name;
}

void Class::setHPP(std::string& content)
{
    this->HPP = content;
}

void Class::setCPP(std::string& content)
{
    this->CPP = content;
}

void Class::setMembers(std::vector<std::pair<std::string, std::string>>& members)
{
    this->members = members;
}

void Class::setMethods(std::vector<std::pair<std::string, std::string>>& methods)
{
    this->methods = methods;
}

void Class::addSettersGetters()
{
    std::regex regex(R"((\n\tprivate\:))");
    for (auto& member : this->members) {
        this->CPP += "void " + this->name + "::set" + this->toUcFirst(member.second) + "(" + member.first + "& " + member.second + ")\n";
        this->CPP += "{\n";
        this->CPP += "\tthis->" + member.second + " = " + member.second + ";\n}\n\n";
        this->CPP += member.first + " " + this->name + "::get" + this->toUcFirst(member.second) + "()\n";
        this->CPP += "{\n";
        this->CPP += "\treturn this->" + member.second + ";\n}\n\n";
        std::string rempacement("\t" + member.first + " get" + this->toUcFirst(member.second) + "();\n$1");
        this->HPP = std::regex_replace(this->HPP, regex, rempacement);
        rempacement = "\tvoid set" + this->toUcFirst(member.second) + "(" + member.first + "& " + member.second + ");\n$1";
        this->HPP = std::regex_replace(this->HPP, regex, rempacement);
    }
}

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

void Class::preview()
{
    std::cout << "\n/// HPP\n\n" << this->HPP << "\n\n/// CPP\n\n" << this->CPP << std::endl;
}

// PRIVATE

std::string Class::toUpperCase(std::string& str)
{
    std::string res = str;
    for (char& c : res) {
        c = std::toupper(c);
    }

    return res;
}

std::string Class::toUcFirst(std::string& str)
{
    std::string res = str;
    res[0] = std::toupper(res[0]);

    return res;
}

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