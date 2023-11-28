#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {

    std::string path = argv[1];
    int size = atoi(argv[2]);
    //std::cout << path << "\t" << size << std::endl;

    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error not open file" << std::endl;
        return 1;
    }

    struct stat fileStats;
    if (fstat(fd, &fileStats) == -1) {
        std::cerr << "Failed to get file information" << std::endl;
        close(fd);
        return 1;
    }

    char* fileData = (char*)mmap(NULL, fileStats.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (fileData == MAP_FAILED) {
        std::cerr << "Failed to map the file into memory" << std::endl;
        close(fd);
        return 1;
    }

    if (fileData[size - 1] == ';' || fileData[size - 1] == '.') {
        std::cout << "Valid: " << fileData << std::endl;
    } else {
        std::cerr << "The string is not valid." << std::endl;
        return 1;
    }

    if (munmap(fileData, fileStats.st_size) == -1) {
        std::cerr << "Failed to free memory" << std::endl;
    }
    close(fd);

    return 0;
}
