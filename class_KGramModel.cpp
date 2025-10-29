#include "KGramModel.h"

#include <iostream>
#include <cstdlib>   // for std::exit, EXIT_FAILURE
#include <cstddef>   // for std::size_t

KGramModel::KGramModel(std::size_t k_, const std::string& text_) : k(k_), text(text_)
{
    if (k == 0) 
    {
        std::cerr << "FATAL: k must be > 0 (k=" << k << ")\n";
        std::exit(EXIT_FAILURE);
    }

    std::size_t text_length = text.size();
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

    std::size_t text_length = text.size();
    for (std::size_t i = 0; i + k <= text_length; ++i) 
    {
        std::string kgram = text.substr(i, k);
        ++kgramCounts[kgram];
        if (i + k < text_length) 
        {
            char nextChar = text[i + k];
            ++transitionCounts[kgram][nextChar];
        }
    }

    computeKGramProbabilities();
    computeTransitionProbabilities();
}

void KGramModel::computeKGramProbabilities()
{
    std::size_t totalCount = 0;
    for (const auto& pair : kgramCounts) 
    {
        totalCount += static_cast<std::size_t>(pair.second);
    }
    if (totalCount == 0) return;

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
        if (it == kgramCounts.end()) continue;
        int total = it->second;
        if (total == 0) continue;
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