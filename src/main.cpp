/**
 * Congratulation for downloading this source code.
 * The sole purpose of this project is to write code for me.
 * Being this lazy to write code is a certain kind of art, innit? 
 */

#include <boost/program_options.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/Class.hpp"
#include "../include/Colour.hpp"
#include "../include/Utils.hpp"

/**
 * @brief Verifies that the specified directory exists. If it doesn't, it is created.
 * 
 * @param dir 
 */
void makeDir(std::string dir) {
	if (!std::filesystem::exists(dir)) {
		if (!std::filesystem::create_directory(dir)) {
			std::cerr << "Error! Cannot create " << dir << std::endl;
			exit(1);
		}
		else {
				std::cout << Colour::Green("* Created " + dir + " directory.") << std::endl;
		}
	}
}

/**
 * @brief Verifies that the specified file exists. If it doesn't, it is created.
 * 
 * @param file 
 * @param fileName 
 */
void makeFile(std::fstream &file, std::string &fileName) {
	file.open(fileName, std::ios::out);
	if (!file.is_open()) {
		std::cerr << "Error! Cannot open " << fileName << std::endl;
		exit(1);
	}
	else {
		std::cout << Colour::Green("* Created " + fileName + " file.") << std::endl;;
	}
}

/**
 * @brief Writes the project's skeleton.
 * 
 * @param projectName 
 */
void makeProject(std::string projectName) {
	std::cout << projectName << std::endl;
	makeDir("src");
	makeDir("include");
	makeDir("build");

	std::fstream fileMain, fileCMake, fileRun;

	std::vector<std::pair<std::fstream *, std::string>> files = {
			{&fileMain, "src/main.cpp"},
			{&fileCMake, "CMakeLists.txt"},
			{&fileRun, "run"}};

	for (auto &file : files) {
		makeFile(*file.first, file.second);
	}

	fileMain << "#include <iostream>\n\nint main(int argc, char "
							"*argv[])\n{\n\tstd::cout << \"Hello World!\" << "
							"std::endl;\n\n\treturn 0;\n}";
	fileCMake << "cmake_minimum_required(VERSION 3.2)\nproject(" << projectName
						<< " LANGUAGES CXX)\n\nset(CMAKE_RUNTIME_OUTPUT_DIRECTORY "
							 "${CMAKE_BINARY_DIR}/bin)\nset(SOURCE_DIR "
							 "${CMAKE_SOURCE_DIR}/src)\n\nfile(GLOB SOURCES "
							 "${SOURCE_DIR}/*.cpp)\n\nadd_executable("
						<< projectName << " ${SOURCES})\n#target_link_libraries("
						<< projectName << " PRIVATE <libs>)\ntarget_compile_features("
						<< projectName
						<< " PRIVATE cxx_std_23)\nadd_compile_options(-Wall "
							 "-Wextra)\n\ninstall(TARGETS "
						<< projectName << ")\n";
	fileRun << "#!/bin/sh\n\ncmake --build build/\n./build/bin/" << projectName;

	for (auto &file : files) {
		file.first->close();
	}

	std::cout << Colour::Green("* Setting up build directory") << std::endl;

	if (std::system("cmake -B build/") != 0) {
		std::cerr << "Error! Cannot launch cmake" << std::endl;
		exit(1);
	}

	if (std::system("chmod a+x run"))
		exit(1);

	std::cout << Colour::Green("* Project created successfully!") << std::endl;
}

/**
 * @brief Display the prompt that asks the user how to build the class.
 * This function also checks if the given class does exist, if it does the files are loaded in the Class object.
 * 
 */
void makeClass() {
	std::cout << Colour::Green("What will be your class name ?\n>");
	std::string className;
	std::cin >> className;
	Class newClass(className);
	std::string path("include/" + newClass.getName() + ".hpp");
	bool fileExists = std::filesystem::exists(path);
	if (fileExists) {
		std::cout << Colour::Green("Class ") +
										 Colour::Yellow("[" + newClass.getName() + "] ") +
										 Colour::Green("already exists. ");
		std::fstream HPP(path, std::ios::in);

		if (!HPP.is_open()) {
			std::cerr << "Error! Cannot open file " << path << std::endl;
			exit(1);
		}

		std::string line, content;

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

	std::cout << Colour::Green("Add methods[") + Colour::Yellow("1") +
									 Colour::Green("] or members[") + Colour::Yellow("2") +
									 Colour::Green("]?\n>");
	std::cin >> option;

	switch (option) {
	case 1:
		while (true) {
			std::cout << Colour::Green("Add a method to the class ") +
											 Colour::Yellow("[" + newClass.getName() + "]") +
											 Colour::Green(" ('s' to save)\n>");
			std::cin >> inputName;

			if (inputName == "s") {
				break;
			}

			std::cout << Colour::Green("What is the return type of ") +
											 Colour::Yellow("[" + inputName + "]") +
											 Colour::Green("?\n>");
			std::cin >> inputReturnType;

			methods.push_back(std::make_pair(inputReturnType, inputName));
		}

		newClass.setMethods(methods);
		break;
	case 2:
		while (true) {
			std::cout << Colour::Green("Add a member to the class ") +
											 Colour::Yellow("[" + newClass.getName() + "]") +
											 Colour::Green(" ('s' to save)\n>");
			std::cin >> inputName;

			if (inputName == "s") {
				break;
			}

			std::cout << Colour::Green("What is the type of ") +
											 Colour::Yellow("[" + inputName + "]") +
											 Colour::Green("?\n>");
			std::cin >> inputReturnType;

			members.push_back(std::make_pair(inputReturnType, inputName));
		}

		newClass.setMembers(members);

		do {
			std::cout << Colour::Green("Generate getters and setters? [") +
											 Colour::Yellow("y") + Colour::Green("/") +
											 Colour::Yellow("n") + Colour::Green("]?\n>");
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
	if (!fileExists) {
		std::cout << Colour::Green("Updating CMake files...") << std::endl;
		std::system("cmake -B build");
	}
}

void makeHeader()
{
	std::string headerName;
	std::cout << Colour::Green("What's the name of the header?\n>");
	std::cin >> headerName;

	std::string fileName = "include/" + Utils::toUcFirst(headerName) + ".hpp";

	std::fstream headerFile;
	makeDir("include");
	makeFile(headerFile, fileName);

	headerFile << 	"#ifndef " + Utils::toUpperCase(headerName) + "_HPP" +
					"\n#define " + Utils::toUpperCase(headerName) + "_HPP" +
					"\n\n\n" +
					"#endif//" + Utils::toUpperCase(headerName) + "_HPP";

	headerFile.close();

	std::cout << Colour::Green("Header created successfully");
}

/**
 * @brief Program's entry point
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[]) {

        std::cout << Colour::Green(
                         R"(
   __________  ____    ________	   ____
  / ____/ __ \/ __ \  / ____/ /   /   /
 / /   / /_/ / /_/ / / /   / /    / /	
/ /___/ ____/ ____/ / /___/ /____/ /	 
\____/_/   /_/	    \____/_____/___/	 
											)") << std::endl; //ASCII art title coloured in green so everyone will think you're a hacker.

        namespace po = boost::program_options;

	po::options_description desc("CPP-CLI");

	desc.add_options()("help,h", "Display help message")(
			"make,m", po::value<std::string>(), "Make (class | header)")(
			"new,n", po::value<std::string>(), "create new project");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << std::endl;
	} else if (vm.count("make")) {
		if (vm["make"].as<std::string>() == "class") {
			makeClass();
		}
		else if (vm["make"].as<std::string>() == "header") {
			makeHeader();
		}
		else {
			std::string wrongCommand = vm["make"].as<std::string>();
			std::cout << Colour::Green(wrongCommand + "...\nReally?\n\n") << Colour::Red("Let's just read the bloody manual and type a valid command, shall we?") << std::endl;
		}
	} else if (vm.count("new")) {
		makeProject(vm["new"].as<std::string>());
	}
	std::cout << "\033[0m" << std::endl;
	return 0;
}