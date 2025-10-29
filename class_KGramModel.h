
#ifndef KGRAMMODEL_H
#define KGRAMMODEL_H

#include <string>
#include <map>
#include <vector>
#include <cstddef> // for std::size_t

class KGramModel
{
protected:
    std::size_t k;
    std::string text;
    std::map<std::string, int> kgramCounts;
    std::map<std::string, std::map<char, int>> transitionCounts;
    std::map<std::string, float> kgramProbabilities;
    std::map<std::string, std::map<char, float>> transitionProbabilities;

public:
    explicit KGramModel(std::size_t k, const std::string& text);
    void train();
    void computeKGramProbabilities();
    void computeTransitionProbabilities();
    std::map<std::string, float> getKGramFrequencies() const;
    std::map<std::string, std::map<char, float>> getTransitionProbabilities() const;
};

#endif // KGRAMMODEL_H