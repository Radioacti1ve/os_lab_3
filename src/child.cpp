#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    std::string fileData = argv[0];
    int size = atoi(argv[1]);
    int i = 0;
    //std::cout << fileData[size - 1] << std::endl;

    // Печатаем содержимое, если оно заканчивается на ';' или '.'
    if (fileData[size - 1] == ';' || fileData[size - 1] == '.') {
        std::cout << "Valid: " << fileData << std::endl;
    } else {
        std::cerr << "The string is not valid." << std::endl;
        return 1;
    }

    return 0;
}
