#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <stdlib.h>
#include <regex>
bool isFormulas(std::string cell){
    const std::regex r("\\=[a-zA-Z]+\\d+(\\+|\\-|\\*|\\/)[a-zA-Z]+\\d+");    
    return std::regex_match(cell, r);
}

bool isNumber(std::string cell){
    const std::regex r("\\d+");    
    return std::regex_match(cell, r);
}

bool isWord(std::string cell){
    const std::regex r("[a-zA-Z]+");    
    return std::regex_match(cell, r);
}



void calculate(std::vector < std::vector < std::string >> & tableData) {
    for (int i = 0; i < tableData.size(); i++) {
        for (int j = 0; j < tableData[i].size(); j++) {
            std::string tempCell = tableData[i][j];
            
            if (isFormulas(tempCell)) {
                int k = 1;
                std::string arg1Letter, arg2Letter, arg1Digit, arg2Digit;
                char operation;
                while (isalpha(tempCell[k])) {
                    arg1Letter.push_back(tempCell[k]);
                    k++;
                }
                while (isdigit(tempCell[k])) {
                    arg1Digit.push_back(tempCell[k]);
                    k++;
                }
                operation = tempCell[k];
                k++;
                while (isalpha(tempCell[k])) {
                    arg2Letter.push_back(tempCell[k]);
                    k++;
                }
                while (isdigit(tempCell[k])) {
                    arg2Digit.push_back(tempCell[k]);
                    k++;
                }
                int a, b;
                int aRow = -1, bRow = -1, aColumn = -1, bColumn = -1;
                for (int i = 0; i < tableData.size(); i++) {
                    if (arg1Digit == tableData[i][0]) {
                        aRow = i;
                        break;
                    }
                }

                for (int i = 0; i < tableData.size(); i++) {
                    if (arg2Digit == tableData[i][0]) {
                        bRow = i;
                        break;
                    }
                }
                for (int j = 0; j < tableData[0].size(); j++) {
                    if (arg1Letter == tableData[0][j]) {
                        aColumn = j;
                        break;
                    }
                }

                for (int j = 0; j < tableData[0].size(); j++) {
                    if (arg2Letter == tableData[0][j]) {
                        bColumn = j;
                        break;
                    }
                }

                a = atoi(tableData[aRow][aColumn].c_str());
                b = atoi(tableData[bRow][bColumn].c_str());
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
                    throw "Invalid operation";
                }
            }
            else if (!isNumber(tempCell)&&(!isWord(tempCell)&&i==0)&&(!tempCell.empty()&&i==0&&j==0)){
                throw ("Invalid cell value");
}
        }
    }
}

void readAndCalculateCsv(std::string fileName) {
    std::string suffix = ".csv";
    if (suffix.length() > fileName.length() || fileName.compare(fileName.length() - suffix.length(), suffix.length(), suffix) != 0) {
        throw "Invalid input. The file must be in csv format";
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
        /*for (int i = 0; i < tableData.size(); i++) {
            for (int j = 0; j < tableData[i].size(); j++) {
                std::cout << tableData[i][j] << ",";
            }
            std::cout << std::endl;
        }*/
    } else {
        throw "File could not open";
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
        throw "Invalid input. The name of the csv file must be passed as the first argument on the command line.";
    }
    std::string fileName = argv[1];
    readAndCalculateCsv(fileName);
    std::string tempStr = "=A1^B1";
    std::cout<<isFormulas(tempStr)<<std::endl;
    return 0;
}
