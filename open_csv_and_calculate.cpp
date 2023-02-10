#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <regex>

bool isFormula(std::string cell) {
    const std::regex r("\\=([a-zA-Z]+)(\\d+)(\\+|\\-|\\*|\\/)([a-zA-Z]+)(\\d+)");

    return std::regex_match(cell, r);
}

std::vector < std::pair < std::string, std::string >> parseArguments(std::string cell) {
    const std::regex r("([a-zA-Z]+)(\\d+)");
    const std::vector < std::smatch > matches {
        std::sregex_iterator {
                cbegin(cell), cend(cell), r
            },
            std::sregex_iterator {}
    };
    std::pair < std::string, std::string > p1(matches[0].str(1), matches[0].str(2));
    std::pair < std::string, std::string > p2(matches[1].str(1), matches[1].str(2));
    std::vector < std::pair < std::string, std::string >> arguments;
    arguments.push_back(p1);
    arguments.push_back(p2);

    return arguments;
}

char parseOperation(std::string cell) {
    const std::regex r("(\\+|\\-|\\*|\\/)");
    std::smatch match;
    regex_search(cell, match, r);
    char operation = match.str(1)[0];
    return operation;
}

bool isNumber(std::string cell) {
    const std::regex r("\\d+");
    return std::regex_match(cell, r);
}

bool isWord(std::string cell) {
    const std::regex r("[a-zA-Z]+");
    return std::regex_match(cell, r);
}

void calculate(std::vector < std::vector < std::string >> & tableData) {
    for (int i = 0; i < tableData.size(); i++) {
        for (int j = 0; j < tableData[i].size(); j++) {
            std::string tempCell = tableData[i][j];

            if (isFormula(tempCell)) {
                std::vector < std::pair < std::string, std::string >> arguments = parseArguments(tempCell);
                char operation = parseOperation(tempCell);
                int k = 1;
                std::string arg1Letter, arg2Letter, arg1Digit, arg2Digit;
                int a, b;
                std::pair < int, int > aIndex(-1, -1);
                std::pair < int, int > bIndex(-1, -1);

                for (int i = 0; i < tableData.size(); i++) {
                    if (arguments[0].second == tableData[i][0]) {
                        aIndex.first = i;
                        break;
                    }
                }

                for (int i = 0; i < tableData.size(); i++) {
                    if (arguments[1].second == tableData[i][0]) {
                        bIndex.first = i;
                        break;
                    }
                }
                for (int j = 0; j < tableData[0].size(); j++) {
                    if (arguments[0].first == tableData[0][j]) {
                        aIndex.second = j;
                        break;
                    }
                }

                for (int j = 0; j < tableData[0].size(); j++) {
                    if (arguments[1].first == tableData[0][j]) {
                        bIndex.second = j;
                        break;
                    }
                }
                if (aIndex.first * aIndex.second * bIndex.first * bIndex.second < 0) {
                    throw "Error: reference to a non-existent cell";
                }
                a = atoi(tableData[aIndex.first][aIndex.second].c_str());
                b = atoi(tableData[bIndex.first][bIndex.second].c_str());
                if (operation == '+') {

                }
                switch (operation) {
                case '+':
                    tableData[i][j] = std::to_string(a + b);
                    break;
                case '-':
                    tableData[i][j] = std::to_string(a - b);
                    break;
                case '*':
                    tableData[i][j] = std::to_string(a * b);
                    break;
                case '/':
                    tableData[i][j] = std::to_string(a / b);
                    break;
                default:
                    throw "Error: invalid operation";
                }
            } else if (!isNumber(tempCell) && (!isWord(tempCell) && i == 0) && (!tempCell.empty() && i == 0 && j == 0)) {
                throw ("Error: invalid cell value");
            }
        }
    }
}

void readAndCalculateCsv(std::string fileName) {
    std::string suffix = ".csv";
    if (suffix.length() > fileName.length() || fileName.compare(fileName.length() - suffix.length(), suffix.length(), suffix) != 0) {
        throw "Error: invalid input. The file must be in csv format";
    }
    std::ifstream file(fileName);
    std::vector < std::vector < std::string >> tableData;
    if (file.is_open()) {
        std::string line, cell;
        std::vector < std::string > vectorLine;
        while (std::getline(file, line)) {
            std::stringstream ssLine(line);
            while (std::getline(ssLine, cell, ',')) {
                vectorLine.push_back(cell);
            }
            tableData.push_back(vectorLine);
            vectorLine.clear();
        }

    } else {
        throw "Error: File could not open";
    }
    file.close();
    calculate(tableData);
    for (int i = 0; i < tableData.size(); i++) {
        for (int j = 0; j < tableData[i].size(); j++) {
            std::cout << tableData[i][j];
            if (j != tableData[i].size() - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
    return;
}

int main(int argc, char * argv[]) {
    if (argc == 1 || argc > 2) {
        throw "Error: invalid input. The name of the csv file must be passed as the first argument on the command line.";
    }
    std::string fileName = argv[1];
    try {
        readAndCalculateCsv(fileName);
    } catch (char const* str) {
        std::cout << str << std::endl;
    }
    return 0;
}
