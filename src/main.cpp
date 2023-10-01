#include <boost/program_options.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../include/Class.hpp"
#include "../include/Color.hpp"

void makeDir(std::string dir) {
  if (!std::filesystem::exists(dir)) {
    if (!std::filesystem::create_directory(dir)) {
      std::cerr << "Error! Cannot create " << dir << std::endl;
      exit(1);
    }
  }
}

void makeFile(std::fstream &file, std::string &fileName) {
  file.open(fileName, std::ios::out);
  if (!file.is_open()) {
    std::cerr << "Error! Cannot open " << fileName << std::endl;
    exit(1);
  }
}

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

  if (std::system("cmake -B build/") != 0) {
    std::cerr << "Error! Cannot launch cmake" << std::endl;
    exit(1);
  }

  if (std::system("chmod a+x run"))
    exit(1);
}

void makeClass() {
  std::cout << Color::Green("What will be your class name ?\n>");
  std::string className;
  std::cin >> className;
  Class newClass(className);
  std::string path("include/" + newClass.getName() + ".hpp");
  bool fileExists = std::filesystem::exists(path);
  if (fileExists) {
    std::cout << Color::Green("Class ") +
                     Color::Yellow("[" + newClass.getName() + "] ") +
                     Color::Green("already exists. ");
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

  std::cout << Color::Green("Add methods[") + Color::Yellow("1") +
                   Color::Green("] or members[") + Color::Yellow("2") +
                   Color::Green("]?\n>");
  std::cin >> option;

  switch (option) {
  case 1:
    while (true) {
      std::cout << Color::Green("Add a method to the class ") +
                       Color::Yellow("[" + newClass.getName() + "]") +
                       Color::Green(" ('s' to save)\n>");
      std::cin >> inputName;

      if (inputName == "s") {
        break;
      }

      std::cout << Color::Green("What is the return type of ") +
                       Color::Yellow("[" + inputName + "]") +
                       Color::Green("?\n>");
      std::cin >> inputReturnType;

      methods.push_back(std::make_pair(inputReturnType, inputName));
    }

    newClass.setMethods(methods);
    break;
  case 2:
    while (true) {
      std::cout << Color::Green("Add a member to the class ") +
                       Color::Yellow("[" + newClass.getName() + "]") +
                       Color::Green(" ('s' to save)\n>");
      std::cin >> inputName;

      if (inputName == "s") {
        break;
      }

      std::cout << Color::Green("What is the type of ") +
                       Color::Yellow("[" + inputName + "]") +
                       Color::Green("?\n>");
      std::cin >> inputReturnType;

      members.push_back(std::make_pair(inputReturnType, inputName));
    }

    newClass.setMembers(members);

    do {
      std::cout << Color::Green("Generate getters and setters? [") +
                       Color::Yellow("y") + Color::Green("/") +
                       Color::Yellow("n") + Color::Green("]?\n>");
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
    std::cout << Color::Green("Updating CMake files...") << std::endl;
    std::system("cmake -B build");
  }
}

int main(int argc, char *argv[]) {

  std::cout << Color::Green(    "   __________  ____     ________    ____\n"
                                    "  / ____/ __ \\/ __ \\   / ____/ /   /  _/\n"
                                    " / /   / /_/ / /_/ /  / /   / /    / /  \n"
                                    "/ /___/ ____/ ____/  / /___/ /____/ /   \n"
                                    "\\____/_/   /_/       \\____/_____/___/   \n"
                                    "                                        ") << std::endl;

  namespace po = boost::program_options;

  po::options_description desc("CPP-CLI");

  desc.add_options()("help,h", "Display help message")(
      "make,m", po::value<std::string>(), "Make [--class]")(
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
  } else if (vm.count("new")) {
    makeProject(vm["new"].as<std::string>());
  }
  std::cout << "\033[0m" << std::endl;
  return 0;
}