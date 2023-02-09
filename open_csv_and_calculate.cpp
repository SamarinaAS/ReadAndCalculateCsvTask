#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

void readAndCalculateCsv(std::string fileName) {
    std::string suffix = ".csv";
    if (suffix.length() > fileName.length() || fileName.compare(fileName.length() - suffix.length(), suffix.length(), suffix) != 0) {
        throw "Invalid input. The file must be in csv format";
    }
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line, cell;
        std::vector < std::vector < std::string >> tableData;
        std::vector<std::string> vectorLine;
        while (std::getline(file, line)) {
            std::stringstream ssLine(line);
            while (std::getline(ssLine, cell, ',')) {
                vectorLine.push_back(cell);
            }
            tableData.push_back(vectorLine);
            vectorLine.clear();
        }
        for (int i = 0; i < tableData.size(); i++) {
            for (int j = 0; j < tableData[i].size(); j++) {                
                std::cout << tableData[i][j] << ",";
            }
            std::cout << std::endl;
        }
    } else {
        throw "File could not open";
    }

    return;
}

int main(int argc, char * argv[]) {
    if (argc == 1 || argc > 2) {
        throw "Invalid input. The name of the csv file must be passed as the first argument on the command line.";
    }
    std::string fileName = argv[1];
    readAndCalculateCsv(fileName);
    return 0;
}
