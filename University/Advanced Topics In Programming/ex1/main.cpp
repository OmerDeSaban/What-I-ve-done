#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>
#include "gamemanager.h"

namespace fs = std::filesystem;
using namespace std;

// taken from chatGPT
vector<string> readInputFile(const string& filename) {
    ifstream infile(filename);
    vector<string> lines;

    if (!infile.is_open()) {
        throw runtime_error("Failed to open input file: " + filename);
    }

    string line;
    while (getline(infile, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    return lines;
}

string getOutputFilename(const string& inputFilename) {
    fs::path inputPath(inputFilename);
    string name = inputPath.filename().string();

    if (name.rfind("input_", 0) == 0 && name.size() > 6 && name.substr(name.size() - 4) == ".txt") {
        return "output_" + name.substr(6);  // Replace 'input_' with 'output_'
    } else {
        throw runtime_error("Invalid input file name: " + inputFilename + ". Expected format: input_x.txt");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "no input file given!\n";
        return 1;
    }

    const char* input_file = argv[1];

    try {
        vector<string> inputLines = readInputFile(input_file);
        GameManager manager;
        manager.setupFromInput(inputLines);
        string outputFileName = getOutputFilename(input_file);
        manager.setOutputFileName(outputFileName);
        manager.startGame();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;

}