#include <boost/program_options.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include "../include/Class.hpp"

void makeDir(std::string dir)
{
		if(!std::filesystem::exists(dir)){
				if (!std::filesystem::create_directory(dir)) {
						std::cerr << "Error! Cannot create " << dir << std::endl;
						exit(1);
				}
		}
}

void makeFile(std::fstream &file, std::string &fileName)
{
		file.open(fileName, std::ios::out);
		if (!file.is_open()) {
				std::cerr << "Error! Cannot open " << fileName << std::endl;
				exit(1);
		}
}

void makeProject(std::string projectName)
{
		std::cout << projectName << std::endl;
		makeDir("src");
		makeDir("include");
		makeDir("build");

		std::fstream fileMain, fileCMake, fileRun;

		std::vector<std::pair<std::fstream*, std::string>> files = {
				{&fileMain, "src/main.cpp"},
				{&fileCMake, "CMakeLists.txt"},
				{&fileRun, "run"}
		};

		for (auto &file : files) {
				makeFile(*file.first, file.second);
		}

		fileMain << "#include <iostream>\n\nint main(int argc, char *argv[])\n{\n\tstd::cout << \"Hello World!\" << std::endl;\n\n\treturn 0;\n}";
		fileCMake << "cmake_minimum_required(VERSION 3.2)\nproject(" << projectName << " LANGUAGES CXX)\n\nset(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)\nset(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)\n\nfile(GLOB SOURCES ${SOURCE_DIR}/*.cpp)\n\nadd_executable(" << projectName << " ${SOURCES})\n#target_link_libraries(" << projectName << " PRIVATE <libs>)\ntarget_compile_features(" << projectName << " PRIVATE cxx_std_23)\nadd_compile_options(-Wall -Wextra)\n\ninstall(TARGETS " << projectName << ")\n";
		fileRun << "#!/bin/sh\n\ncmake --build build/\n./build/bin/" << projectName;

		for (auto &file : files) {
				file.first->close();
		}

		if(std::system("cmake -B build/") != 0){
				std::cerr << "Error! Cannot launch cmake" << std::endl;
				exit(1);
		}

		if(std::system("chmod a+x run"))
				exit(1);
}

void makeClass() {
	std::cout << "\033[32mWhat will be your class name ?\n>\033[0m";
	std::string className;
	std::cin >> className;
	Class newClass(className);
	std::string path("include/" + newClass.getName() + ".hpp");
	if (std::filesystem::exists(path)) {
		std::cout << "\033[32mClass \033[33m[" + newClass.getName() + "] \033[32malready exists. ";
		std::fstream HPP(path, std::ios::in);
		if (!HPP.is_open()) {
				std::cerr << "Error! Cannot open file " << path << std::endl;
				exit(1);
		}
		std::string line;
		std::string content;
		while (std::getline(HPP, line)) {
				content += line + "\n";
		}
		newClass.setHPP(content);
		HPP.close();

		path = "src/" + newClass.getName() + ".cpp";
		std::fstream CPP(path);
		if (!CPP.is_open()) {
				std::cerr << "Error! Cannot open file " << path << std::endl;
				exit(1);
		}
		line.clear();
		content.clear();
		while (std::getline(CPP, line)) {
				content += line + "\n";
		}
		newClass.setCPP(content);
		CPP.close();
	}
	std::vector<std::pair<std::string, std::string>> methods, members;
	std::string inputName, inputReturnType, choice;

	int option;
	std::cout << "\033[32mAdd methods[\033[33m1\033[32m] or members[\033[33m2\033[32m] ? \033[0m";
	std::cin >> option;
	switch (option) {
	case 1:
		while (true) {
			std::cout << "\033[32mAdd a method to the class \033[33m[" + newClass.getName() + "]\033[32m ('s' to save)\n>\033[0m";
			std::cin >> inputName;

			if (inputName == "s") {
				break;
			}

			std::cout << "\033[32mWhat is the return type of \033[33m[" + inputName + "]\033[32m?\n>\033[0m";
			std::cin >> inputReturnType;

			methods.push_back(std::make_pair(inputReturnType, inputName));
		}

		newClass.setMethods(methods);
		break;
	case 2:
		while (true) {
			std::cout << "\033[32mAdd a member to the class \033[33m[" + newClass.getName() + "]\033[32m ('s' to save)\n>\033[0m";
			std::cin >> inputName;

			if (inputName == "s") {
				break;
			}

			std::cout << "\033[32mWhat is the type of \033[33m[" + inputName + "]\033[32m?\n>\033[0m";
			std::cin >> inputReturnType;

			members.push_back(std::make_pair(inputReturnType, inputName));
		}

		newClass.setMembers(members);

		do {
			std::cout << "\033[32mGenerate getters and setters? [y/n] \033[0m";
			std::cin >> choice;
		} while (!std::cin.fail() && choice != "y" && choice != "n");

		if (choice == "y") {
			newClass.addSettersGetters();
		}
		break;
	default:
		// todo
		break;
	}
    newClass.generate();
}

int main(int argc, char *argv[]) {
	namespace po = boost::program_options;

	po::options_description desc("CPP-CLI");

	desc.add_options()("help,h", "Display help message")
		("make,m", po::value<std::string>(), "Make [--class]")
		("new,n", po::value<std::string>(), "create new project");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
	} else if (vm.count("make")) {
		if (vm["make"].as<std::string>() == "class") {
				makeClass();
		}
	} else if (vm.count("new")) {
		makeProject(vm["new"].as<std::string>());
	}
    std::cout << "\033[0m" << std::endl;
	return 0;
}