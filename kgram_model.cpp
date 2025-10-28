
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>   // for std::exit, EXIT_FAILURE


class KGramModel 
{
protected:
    int k;
    std::string text;
    std::map<std::string, int> kgramCounts;
    std::map<std::string, std::map<char, int>> transitionCounts;
    std::map<std::string, float> kgramProbabilities;
    std::map<std::string, std::map<char, float>> transitionProbabilities;

public:
    KGramModel(int k, const std::string& text);
    void train();
    void computeKGramProbabilities();
    void computeTransitionProbabilities();
    std::map<std::string, float> getKGramFrequencies() const;
    std::map<std::string, std::map<char, float>> getTransitionProbabilities() const;
};

KGramModel::KGramModel(int k_, const std::string& text_) : k(k_), text(text_) 
{   
    if (k <= 0) 
    {        
        std::cerr << "FATAL: k must be > 0 (k=" << k << ")\n";        
        std::exit(EXIT_FAILURE);    
    }    

    int text_length = static_cast<int>(text.size());    
    if (text_length < k) 
    {        
        std::cerr << "FATAL: text length (" << text_length << ") < k (" << k << ")\n";        
        std::exit(EXIT_FAILURE);    
    }

    train();
}

void KGramModel::train() 
{
    kgramCounts.clear();
    transitionCounts.clear();
    kgramProbabilities.clear();
    transitionProbabilities.clear();

    for (int i = 0; i + k <= text_length; ++i) 
    {
        std::string kgram = text.substr(i, k);
        kgramCounts[kgram]++;
        if (i + k < text_length) 
        {
            char nextChar = text[i + k];
            transitionCounts[kgram][nextChar]++;
        }
    }

    computeKGramProbabilities();
    computeTransitionProbabilities();
}

void KGramModel::computeKGramProbabilities() 
{
    int totalCount = 0;
    for (const auto& pair : kgramCounts) 
    {
        totalCount += pair.second;
    }

    for (const auto& pair : kgramCounts) 
    {
        kgramProbabilities[pair.first] = static_cast<float>(pair.second) / static_cast<float>(totalCount);
    }
}

void KGramModel::computeTransitionProbabilities() 
{
    for (const auto& elem : transitionCounts) 
    {
        const std::string& kgram = elem.first;
        auto it = kgramCounts.find(kgram);
        int total = it->second;
        for (const auto& p : elem.second) 
        {
            transitionProbabilities[kgram][p.first] = static_cast<float>(p.second) / static_cast<float>(total);
        }
    }
}

std::map<std::string, float> KGramModel::getKGramFrequencies() const 
{
    return kgramProbabilities;
}

std::map<std::string, std::map<char, float>> KGramModel::getTransitionProbabilities() const 
{
    return transitionProbabilities;
}

#include <random>
#include <ctime>
#include <cmath>      // for std::fabs

class TextGenerator : public KGramModel 
{
private:
    std::mt19937 rng;
    std::string chooseMaxProbabilityKGram();
    char chooseMaxProbabilityNextChar(const std::string& kgram);

public:
    TextGenerator(int k, const std::string& text);
    std::string generateText(size_t length);
};

TextGenerator::TextGenerator(int k, const std::string& text)
    : KGramModel(k, text), rng(static_cast<unsigned int>(std::time(nullptr))) { }

std::string TextGenerator::chooseMaxProbabilityKGram() 
{
    if (kgramProbabilities.empty()) return std::string(); // empty

    float maxProb = -1.0f;
    std::vector<std::string> candidateKGrams;
    for (const auto& pair : kgramProbabilities) 
    {
        if (pair.second > maxProb) 
        {
            maxProb = pair.second;
            candidateKGrams.clear();
            candidateKGrams.push_back(pair.first);
        } 
        else if (pair.second == maxProb) 
        {
            candidateKGrams.push_back(pair.first);
        }
    }
    if (candidateKGrams.empty()) 
    {
        std::cerr << "Fatal error: no candidate k-gram found. Aborting.\n";
        std::exit(EXIT_FAILURE);
    }
    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidateKGrams[dist(rng)];
}

char TextGenerator::chooseMaxProbabilityNextChar(const std::string& kgram) 
{
    auto it = transitionProbabilities.find(kgram);
    if (it == transitionProbabilities.end() || it->second.empty()) 
    {    
        std::cerr << "Fatal error: no transitions for k-gram \"" << kgram              << "\". Aborting.\n";    
        std::exit(EXIT_FAILURE);
    }

    const float EPS = 1e-7f;
    float maxProb = -1.0f;
    std::vector<char> candidateNextChars;
    for (const auto& pair : it->second) 
    {
        if (pair.second > maxProb + EPS) {
            maxProb = pair.second;
            candidateNextChars.clear();
            candidateNextChars.push_back(pair.first);
        } 
        else if (std::fabs(pair.second - maxProb) <= EPS) 
        {
            candidateNextChars.push_back(pair.first);
        }
    }
    if (candidateNextChars.empty()) 
    {
        std::cerr << "Fatal error: no candidate NextChar found. Aborting.\n";
        std::exit(EXIT_FAILURE);
    }
    std::uniform_int_distribution<size_t> dist(0, candidateNextChars.size() - 1);
    return candidateNextChars[dist(rng)];
}

std::string TextGenerator::generateText(size_t length) 
{
    if (length == 0) return std::string();
    if (kgramProbabilities.empty()) return std::string(); // nothing to base on

    std::string current = chooseMaxProbabilityKGram();
    if (current.empty()) return std::string();

    // If the chosen k-gram is already longer than requested, return a prefix.
    if (current.size() >= length) return current.substr(0, length);

    std::string result = current;
    while (result.size() < length) 
    {
        char next = chooseMaxProbabilityNextChar(current);
        if (next == '\0') 
        {    
            std::cerr << "Fatal error: chooseMaxProbabilityNextChar returned NUL for k-gram \""
                      << current << "\". Stopping generation.\n";    
            break; 
        }
        result.push_back(next);
        // update k-gram
        if (k > 0) current = current.substr(1) + next;
    }

    if (result.size() > length) result.resize(length);
    return result;
}


#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <stdexcept>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <k> <filename> <length>\n";
        return 1;
    }

    int k = 0;
    std::size_t length = 0;
    try {
        k = std::stoi(argv[1]);
        if (k < 0) throw std::invalid_argument("k must be non-negative");
        length = static_cast<std::size_t>(std::stoll(argv[3]));
    } catch (const std::exception& e) {
        std::cerr << "Invalid numeric argument: " << e.what() << '\n';
        return 1;
    }

    std::string filename = argv[2];
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << '\n';
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (text.empty()) {
        std::cerr << "Input file is empty.\n";
        return 1;
    }
    if (static_cast<std::size_t>(k) > text.size()) {
        std::cerr << "k is larger than input text length.\n";
        return 1;
    }

    TextGenerator gen(k, text);
    std::string generated_text = gen.generateText(length);
    std::cout << generated_text << '\n';
    return 0;
}