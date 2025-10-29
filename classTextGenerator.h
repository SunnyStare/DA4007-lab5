#ifndef TEXTGENERATOR_H
#define TEXTGENERATOR_H

#include "classKGramModel.h"
#include <random>
#include <string>
#include <cstddef> // for std::size_t

class TextGenerator : public KGramModel
{
private:
    mutable std::mt19937 rng;

    // Select the k-gram with the highest probability (does not modify object state)
    std::string chooseMaxProbabilityKGram() const;
    // Given a k-gram, select the next character with the highest transition probability
    char chooseMaxProbabilityNextChar(const std::string& kgram) const;

public:
    explicit TextGenerator(std::size_t k, const std::string& text);

    // Generate text of the specified length
    std::string generateText(std::size_t length);
};

#endif // TEXTGENERATOR_H