#include <calculate_csv.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>

struct CellAddress {
    std::string column;
    int row;

    CellAddress(std::string column, int row) : column(column), row(row) {}
    CellAddress(std::string cellAddress) {
        const std::regex cellAddressTemplate("([a-zA-Z]+)(\\d+)");
        const std::vector<std::smatch> matches{
            std::sregex_iterator{cbegin(cellAddress), cend(cellAddress), cellAddressTemplate}, std::sregex_iterator{}};
        this->column = matches[0].str(1);
        this->row = atoi(matches[0].str(2).c_str());
    }
};

struct Formula {
    std::pair<int, int> resultCelliIndices;
    std::pair<std::string, std::string> arguments;
    char operation;

    Formula(std::pair<int, int> resultCelliIndices, std::pair<std::string, std::string> arguments, char operation)
        : resultCelliIndices(resultCelliIndices), arguments(arguments), operation(operation) {}

    Formula(std::pair<int, int> resultCelliIndices, std::string cell) {
        const std::regex formulaTemplate("\\=(\\w+)(\\+|\\-|\\*|\\/)(\\w+)");
        const std::vector<std::smatch> matches{std::sregex_iterator{cbegin(cell), cend(cell), formulaTemplate},
                                               std::sregex_iterator{}};
        this->resultCelliIndices = resultCelliIndices;
        this->arguments = std::make_pair(matches[0].str(1), matches[0].str(3));
        this->operation = matches[0].str(2)[0];
    }
};

bool isFormula(std::string cell) {
    const std::regex formulaTemplate("\\=((([a-zA-Z]+)(\\d+))|(\\d+))(\\+|\\-|\\*|\\/)((([a-zA-Z]+)(\\d+))|(\\d+))");
    return std::regex_match(cell, formulaTemplate);
}

bool isNumber(std::string cell) {
    const std::regex number("\\d+");
    return std::regex_match(cell, number);
}

bool isWord(std::string cell) {
    const std::regex word("[a-zA-Z]+");
    return std::regex_match(cell, word);
}

std::pair<int, int> getIndicesCell(CellAddress argument, std::vector<std::vector<std::string>> &tableData) {
    std::pair<int, int> indices(-1, -1);
    for (long unsigned int j = 0; j < tableData[0].size(); j++) {
        if (argument.column == tableData[0][j]) {
            indices.second = j;
            break;
        }
    }
    for (long unsigned int i = 0; i < tableData.size(); i++) {
        if (std::to_string(argument.row) == tableData[i][0]) {
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

bool isTableCorrect(std::vector<std::vector<std::string>> &tableData) {
    for (long unsigned int i = 0; i < tableData.size(); i++) {
        if (tableData[i].size() != tableData[0].size()) {
            return false;
        }
        for (long unsigned int j = 0; j < tableData[i].size(); j++) {
            if (!(isFormula(tableData[i][j]) && i != 0 && j != 0) &&
                !(isNumber(tableData[i][j]) && i != 0 && (j != 0 || atoi(tableData[i][j].c_str()) > 0)) &&
                !(isWord(tableData[i][j]) && i == 0) && !(tableData[i][j].empty() && i == 0 && j == 0)) {
                return false;
            }
        }
    }
    return true;
}

void readCsv(std::string fileName, std::vector<std::vector<std::string>> &tableData) {
    std::string suffix = ".csv";
    if (suffix.length() > fileName.length() ||
        fileName.compare(fileName.length() - suffix.length(), suffix.length(), suffix) != 0) {
        throw "Error: invalid input. The file must be in csv format";
    }
    std::ifstream file(fileName);
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
    return;
}

std::string getValue(const std::vector<std::vector<std::string>> &tableData, const std::pair<int, int> &cellIndex) {
    return tableData[cellIndex.first][cellIndex.second];
}

bool isCellAddressCorrect(std::pair<int, int> cellAddress) {
    if (cellAddress.first < 0 || cellAddress.second < 0) {
        return false;
    }
    return true;
}

void calculateTable(std::vector<std::vector<std::string>> &tableData) {
    if (!isTableCorrect(tableData)) {
        throw("Error: table is set incorrectly");
    }
    for (long unsigned int i = 0; i < tableData.size(); i++) {
        for (long unsigned int j = 0; j < tableData[i].size(); j++) {
            std::string tempCell = tableData[i][j];
            if (isFormula(tempCell)) {
                int a, b;
                std::pair<int, int> aIndex(-1, -1);
                std::pair<int, int> bIndex(-1, -1);
                std::stack<Formula> stack;
                bool isArg1Correct = false;
                bool isArg2Correct = false;

                stack.push(Formula(std::make_pair(i, j), tempCell));
                while (!stack.empty()) {
                    if (stack.size() > tableData[0].size() * tableData.size()) {
                        throw "Error: unable to calculate formulas in the table, there was a looping of formulas";
                    }
                    if (isNumber(stack.top().arguments.first)) {
                        a = atoi(stack.top().arguments.first.c_str());
                        isArg1Correct = true;
                    } else {
                        aIndex = getIndicesCell(CellAddress(stack.top().arguments.first), tableData);
                        isArg1Correct = isCellAddressCorrect(aIndex);
                    }
                    if (isNumber(stack.top().arguments.second)) {
                        b = atoi(stack.top().arguments.second.c_str());
                        isArg2Correct = true;
                    } else {
                        bIndex = getIndicesCell(CellAddress(stack.top().arguments.second), tableData);
                        isArg2Correct = isCellAddressCorrect(bIndex);
                    }
                    if (!(isArg1Correct && isArg2Correct)) {
                        throw "Error: reference to a non-existent cell";
                    }

                    if (!isNumber(stack.top().arguments.first) && isFormula(getValue(tableData, aIndex))) {
                        stack.push(Formula(aIndex, getValue(tableData, aIndex)));

                    } else if (!isNumber(stack.top().arguments.second) && isFormula(getValue(tableData, bIndex))) {
                        stack.push(Formula(bIndex, getValue(tableData, bIndex)));
                    } else {
                        a = isNumber(stack.top().arguments.first) ? a : atoi(getValue(tableData, aIndex).c_str());
                        b = isNumber(stack.top().arguments.second) ? b : atoi(getValue(tableData, bIndex).c_str());
                        tableData[stack.top().resultCelliIndices.first][stack.top().resultCelliIndices.second] =
                            std::to_string(calculateExpression(a, b, stack.top().operation));
                        stack.pop();
                    }
                }
            }
        }
    }
}

void printTable(std::vector<std::vector<std::string>> &tableData) {
    for (long unsigned int i = 0; i < tableData.size(); i++) {
        for (long unsigned int j = 0; j < tableData[i].size(); j++) {
            std::cout << tableData[i][j];
            if (j != tableData[i].size() - 1) {
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
}