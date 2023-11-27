#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    // Проверка наличия аргумента (пути к файлу)
    if(argc != 2) {
        std::cerr << "Opening file error" << std::endl;
        return 1;
    }

    const char* CHILD = "child";

    // Формирование пути к файлу
    std::string path {"../test/"};
    std::string name {argv[1]};
    path += name;

    // Открытие файла
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error not open file" << std::endl;
        return 1;
    }

    // Определение размера файла
    struct stat fileStats;
    if(fstat(fd, &fileStats) == -1) {
        std::cerr << "Failed to get file information" << std::endl;
        close(fd);
        return 1;
    }

    // Отображение файла в память
    char* fileData = (char*)mmap(NULL, fileStats.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if(fileData == MAP_FAILED) {
        std::cerr << "Failed to map the file into memory" << std::endl;
        close(fd);
        return 1;
    }

    // Создание дочернего процесса
    pid_t pid = fork();

    if(pid == -1) {
        perror("Fork Error");
        munmap(fileData, fileStats.st_size);
        close(fd);
        return 1;
    }

    if(pid == 0) {
        // Код для дочернего процесса (заменяется на другую программу)
        execl(CHILD, CHILD, path.c_str(), std::to_string(fileStats.st_size).c_str(), nullptr);
        perror("Execl Error");
        munmap(fileData, fileStats.st_size);
        return 1;
    } else {
        // Код для родительского процесса
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status)) {
            std::cout << "Child process was finished by code: " << WEXITSTATUS(status) << std::endl;
        }
    }

    // Освобождение памяти и закрытие файла
    if(munmap(fileData, fileStats.st_size) == -1) {
        std::cerr << "Failed to free memory" << std::endl;
    }
    close(fd);

    return 0;
}
