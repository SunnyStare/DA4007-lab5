
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <k> <filename> <length>" << std::endl;
        return 1;
    }

    int k = std::stoi(argv[1]);
    std::string filename = argv[2];
    int length = std::stoi(argv[3]);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }

    do the work

    return 0;
}