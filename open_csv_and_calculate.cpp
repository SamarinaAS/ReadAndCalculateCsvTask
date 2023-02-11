#include <ctype.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

struct FormulaCell {
    std::pair<int, int> resultCelliIndices;
    std::pair<std::string, std::string> argument1;
    std::pair<std::string, std::string> argument2;
    char operation;

    FormulaCell(std::pair<int, int> resultCelliIndices,
                std::pair<std::string, std::string> argument1,
                std::pair<std::string, std::string> argument2, char operation) {
        this->resultCelliIndices = resultCelliIndices;
        this->argument1 = argument1;
        this->argument2 = argument2;
        this->operation = operation;
    }

    FormulaCell(std::pair<std::string, std::string> argument1,
                std::pair<std::string, std::string> argument2, char operation) {
        this->resultCelliIndices = std::make_pair(-1, -1);
        this->argument1 = argument1;
        this->argument2 = argument2;
        this->operation = operation;
    }
};

bool isFormula(std::string cell) {
    const std::regex r(
        "\\=([a-zA-Z]+)(\\d+)(\\+|\\-|\\*|\\/)([a-zA-Z]+)(\\d+)");

    return std::regex_match(cell, r);
}

FormulaCell parseFormula(std::string cell) {
    const std::regex arg("([a-zA-Z]+)(\\d+)");
    const std::vector<std::smatch> matches{
        std::sregex_iterator{cbegin(cell), cend(cell), arg},
        std::sregex_iterator{}};

    const std::regex oper("(\\+|\\-|\\*|\\/)");
    std::smatch match;
    regex_search(cell, match, oper);
    FormulaCell parsedFormula(
        std::make_pair(matches[0].str(1), matches[0].str(2)),
        std::make_pair(matches[1].str(1), matches[1].str(2)), match.str(1)[0]);

    return parsedFormula;
}

bool isNumber(std::string cell) {
    const std::regex r("\\d+");
    return std::regex_match(cell, r);
}

bool isWord(std::string cell) {
    const std::regex r("[a-zA-Z]+");
    return std::regex_match(cell, r);
}

std::pair<int, int> indicesCell(std::pair<std::string, std::string> argument, std::vector<std::vector<std::string>> &tableData) {
    std::pair<int, int> indices(-1, -1);
    for (int j = 0; j < tableData[0].size(); j++) {
        if (argument.first == tableData[0][j]) {
            indices.second = j;
            break;
        }
    }
    for (int i = 0; i < tableData.size(); i++) {
        if (argument.second == tableData[i][0]) {
            indices.first = i;
            break;
        }
    }
    return indices;
}

int calculateExpression(int a, int b, char operation) {
    int result;
    switch (operation) {
        case '+':
            result = a + b;
            break;
        case '-':
            result = a - b;
            break;
        case '*':
            result = a * b;
            break;
        case '/':
            if (b == 0) {
                throw "Error: division by zero";
            }
            result = a / b;
            break;
        default:
            throw "Error: invalid operation";
    }
    return result;
}

void parseAndCalculateTable(std::vector<std::vector<std::string>> &tableData) {
    for (int i = 0; i < tableData.size(); i++) {
        for (int j = 0; j < tableData[i].size(); j++) {
            std::string tempCell = tableData[i][j];
            if (isFormula(tempCell)) {
                // std::cout<<tempCell<<std::endl;
                // std::cout<<isNumber(tempCell)<<std::endl;
                //  std::vector < std::pair < std::string, std::string >>
                //  arguments = parseArguments(tempCell);
                FormulaCell parsedCell = parseFormula(tempCell);
                // auto arguments = parseArguments(tempCell);
                char operation = parsedCell.operation;
                int k = 1;
                std::string arg1Letter, arg2Letter, arg1Digit, arg2Digit;
                int a, b;
                std::pair<int, int> aIndex(-1, -1);
                std::pair<int, int> bIndex(-1, -1);

                std::stack<FormulaCell> stack;
                parsedCell.resultCelliIndices = std::make_pair(i, j);
                stack.push(parsedCell);
                while (!stack.empty()) {
                    if (stack.size() > tableData[0].size() * tableData.size() - 1) {
                        throw "Error: unable to calculate formulas in the table, there was a looping of formulas";
                    }
                    aIndex = indicesCell(stack.top().argument1, tableData);
                    bIndex = indicesCell(stack.top().argument2, tableData);
                    if (aIndex.first * aIndex.second * bIndex.first * bIndex.second < 0) {
                        throw "Error: reference to a non-existent cell";
                    }
                    if (isFormula(tableData[aIndex.first][aIndex.second])) {
                        parsedCell = parseFormula(tableData[aIndex.first][aIndex.second]);
                        parsedCell.resultCelliIndices = std::make_pair(aIndex.first, aIndex.second);
                        stack.push(parsedCell);

                    } else if (isFormula(tableData[bIndex.first][bIndex.second].c_str())) {
                        parsedCell = parseFormula(tableData[bIndex.first][bIndex.second]);
                        parsedCell.resultCelliIndices = std::make_pair(bIndex.first, bIndex.second);
                        stack.push(parsedCell);
                    } else {
                        a = atoi(tableData[aIndex.first][aIndex.second].c_str());
                        b = atoi(tableData[bIndex.first][bIndex.second].c_str());
                        tableData[stack.top().resultCelliIndices.first][stack.top().resultCelliIndices.second] = std::to_string(calculateExpression(a, b, operation));
                        stack.pop();
                    }
                }
            } else if (!isNumber(tempCell) && !(isWord(tempCell) && i == 0) &&
                       !(tempCell.empty() && i == 0 && j == 0)) {
                throw("Error: invalid cell value");
            }
        }
    }
}

void readAndCalculateCsv(std::string fileName) {
    std::string suffix = ".csv";
    if (suffix.length() > fileName.length() ||
        fileName.compare(fileName.length() - suffix.length(), suffix.length(),
                         suffix) != 0) {
        throw "Error: invalid input. The file must be in csv format";
    }
    std::ifstream file(fileName);
    std::vector<std::vector<std::string>> tableData;
    if (file.is_open()) {
        std::string line, cell;
        std::vector<std::string> vectorLine;
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
    parseAndCalculateTable(tableData);
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

int main(int argc, char *argv[]) {
    if (argc == 1 || argc > 2) {
        throw "Error: invalid input. The name of the csv file must be passed as the first argument on the command line.";
    }
    std::string fileName = argv[1];
    try {
        readAndCalculateCsv(fileName);
    } catch (char const *str) {
        std::cout << str << std::endl;
    }
    return 0;
}
