


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sstream>


class KGramModel 
{
protected:
    int k; 
    std::string text;
    std::map<std::string, int> kgramCounts; 
    std::map<std::string, std::map<char, int>> transitionCounts; 
    void computekgramProbabilities();
    void computeTransitionProbabilities();
public:
    KGramModel::KGramModel(int k, const std::string& text);{}
    void train(const std::string& text);
    std::map<std::string, float> kgramProbabilities; 
    std::map<std::string, std::map<char, float>> TransitionProbabilities;
    std::map<std::string, float> getKGramFrequencies() const;
    std::map<std::string, std::map<char, float>> getTransitionProbabilities() const;
};

KGramModel::KGramModel(int k, const std::string& text) : k(k) 
{    
    train(text); 
}

void KGramModel::train(const std::string& text) 
{
    int text_length = text.size();
    for (int i = 0; i <= text_length - k; ++i) 
    {
        std::string kgram = text.substr(i, k);
        kgramCounts[kgram]++;

        if (i + k < text_length) 
        {
            char nextChar = text[i + k];
            transitionCounts[kgram][nextChar]++;
        }
    }
    computekgramProbabilities();
    computeTransitionProbabilities();
}

void KGramModel::computeKGramProbabilities() 
{
    int totalCount = 0;
    for (const auto& pair : kgramCounts) 
    {
        totalCount += pair.second; 
    }

    
    for (const auto& pair : kgramCounts) {
        const std::string& kgram = pair.first; 
        int count = pair.second; 
        
        if (totalCount > 0) 
        { 
            kgramProbabilities[kgram] = static_cast<float>(count) / static_cast<float>(totalKGramCount);
        } 
        else 
        {
            std::cerr << "Error: total k-gram count is zero! k is bigger than textlength!" << std::endl;
        }
    }
}

std::map<std::string, float> KGramModel::getKGramFrequencies() const 
{
    return kgramProbabilities;
}

void computeTransitionProbabilities()
{
    for (const auto& elem : transitionCounts) 
    {
        int total = 0; 
        auto it = kgramCounts.begin(); 
        while (it != kgramCounts.end()) 
        {            
            if (it->first == elem.first) 
            { 
                total = it->second; 
                break;
            }            
        it++; 
        }
        //'total' is ready now.

        if (total == 0)         
        {           
            std::cerr << "kgramCounts or transitionCounts have error. Some kgrams in transitionCounts are not in kgramCounts!" << std::endl;
            break;
        }

        for (const auto& pair : elem.second) 
        {
            TransitionProbabilities[elem.first][pair.first] = static_cast<float>(pair.second) / static_cast<float>(total);
        }
    }
}

std::map<std::string, std::map<char, float>> KGramModel::getTransitionProbabilities() const 
{
    return TransitionProbabilities;
}

