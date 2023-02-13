#include <calculate_csv.h>

#include <iostream>

int main(int argc, char *argv[]) {
    try {
        if (argc == 1 || argc > 2) {
            throw "Error: invalid input. The name of the csv file must be passed as the first argument on the command line.";
        }
        std::string fileName = argv[1];

        std::vector<std::vector<std::string>> tableData;
        ReadCsv(fileName, tableData);
        calculateTable(tableData);
        printTable(tableData);
    } catch (char const *str) {
        std::cout << str << std::endl;
    }
    return 0;
}
