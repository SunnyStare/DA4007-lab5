#include "TextGenerator.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <stdexcept>


int main(int argc, char* argv[]) 
{
    if (argc != 4) 
    {
        std::cerr << "Usage: " << argv[0] << " <k> <filename> <length>\n";
        return 1;
    }

    std::size_t k = 0;
    std::size_t length = 0;
    try 
    {
        long long k_tmp = std::stoll(argv[1]);
        if (k_tmp <= 0) throw std::invalid_argument("k must be > 0");
        k = static_cast<std::size_t>(k_tmp);

        unsigned long long len_tmp = std::stoull(argv[3]);
        length = static_cast<std::size_t>(len_tmp);
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Invalid numeric argument: " << e.what() << '\n';
        return 1;
    }

    std::string filename = argv[2];
    std::ifstream file(filename, std::ios::binary);
    if (!file) 
    {
        std::cerr << "Error opening file: " << filename << '\n';
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (text.empty()) 
    {
        std::cerr << "Input file is empty.\n";
        return 1;
    }
    if (k > text.size()) {
        std::cerr << "k is larger than input text length.\n";
        return 1;
    }

    TextGenerator gen(k, text);
    std::string generated_text = gen.generateText(length);
    std::cout << generated_text << '\n';
    return 0;
}