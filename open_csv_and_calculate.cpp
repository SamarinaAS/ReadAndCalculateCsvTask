#include <iostream>
#include <string>

void readAndCalculateCsv(std::string fileName){
    std::cout<<fileName<<std::endl;
    return;
}

int main (int argc, char* argv[]){
    if (argc == 1 || argc > 2){
        throw "Invalid argument. The name of the csv file must be passed as the first argument on the command line.";
    }
    std::string fileName = argv[1];
    readAndCalculateCsv(fileName);
    return 0;
}

