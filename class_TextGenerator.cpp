#include "TextGenerator.h"
#include <iostream>
#include <vector>
#include <cstdlib>   // for std::exit, EXIT_FAILURE
#include <random>
#include <ctime>
#include <cmath>      // for std::fabs
#include <cstddef>    // for std::size_t

TextGenerator::TextGenerator(std::size_t k, const std::string& text)
    : KGramModel(k, text),
      rng(static_cast<unsigned int>(std::time(nullptr)))
{}

std::string TextGenerator::chooseMaxProbabilityKGram() const
{
    if (kgramProbabilities.empty()) return std::string();

    float maxProb = -1.0f;
    std::vector<std::string> candidateKGrams;
    for (const auto& pair : kgramProbabilities) {
        if (pair.second > maxProb) {
            maxProb = pair.second;
            candidateKGrams.clear();
            candidateKGrams.push_back(pair.first);
        } else if (pair.second == maxProb) {
            candidateKGrams.push_back(pair.first);
        }
    }

    if (candidateKGrams.empty()) {
        std::cerr << "Fatal error: no candidate k-gram found. Aborting.\n";
        std::exit(EXIT_FAILURE);
    }

    std::uniform_int_distribution<std::size_t> dist(0, candidateKGrams.size() - 1);
    return candidateKGrams[dist(rng)];
}

char TextGenerator::chooseMaxProbabilityNextChar(const std::string& kgram) const
{
    auto it = transitionProbabilities.find(kgram);
    if (it == transitionProbabilities.end() || it->second.empty()) {
        std::cerr << "Fatal error: no transitions for k-gram \"" << kgram << "\". Aborting.\n";
        std::exit(EXIT_FAILURE);
    }

    const float EPS = 1e-7f;
    float maxProb = -1.0f;
    std::vector<char> candidateNextChars;
    for (const auto& pair : it->second) {
        if (pair.second > maxProb + EPS) {
            maxProb = pair.second;
            candidateNextChars.clear();
            candidateNextChars.push_back(pair.first);
        } else if (std::fabs(pair.second - maxProb) <= EPS) {
            candidateNextChars.push_back(pair.first);
        }
    }

    if (candidateNextChars.empty()) {
        std::cerr << "Fatal error: no candidate next char found. Aborting.\n";
        std::exit(EXIT_FAILURE);
    }

    std::uniform_int_distribution<std::size_t> dist(0, candidateNextChars.size() - 1);
    return candidateNextChars[dist(rng)];
}

std::string TextGenerator::generateText(std::size_t length)
{
    if (length == 0) return std::string();
    if (kgramProbabilities.empty()) return std::string();

    std::string current = chooseMaxProbabilityKGram();
    if (current.empty()) return std::string();

    if (current.size() >= length) return current.substr(0, length);

    std::string result = current;
    while (result.size() < length) {
        char next = chooseMaxProbabilityNextChar(current);
        result.push_back(next);
        if (k > 0) current = current.substr(1) + next;
    }

    if (result.size() > length) result.resize(length);
    return result;
}