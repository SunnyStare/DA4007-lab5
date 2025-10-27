
#include "kgram_model.h"
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

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
     get the chart(k, file) 

     textgenerater(chart, length)

    std::string generated_text = model.generate(length);
    std::cout << generated_text << std::endl;

    return 0;
}