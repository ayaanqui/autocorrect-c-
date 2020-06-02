#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Autocorrect
{
private:
    const std::vector<char> delimiters = {' ', ',', '.', '(', ')', '?', '"', '\'', '-', '!', ';'};
    std::vector<char> alphabets = std::vector<char>(26);
    std::vector<std::string> parsedText;
    std::unordered_map<std::string, std::string> wordsTable;

    std::string toLower(std::string str)
    {
        // Convert to lowercase
        // Reference: https://stackoverflow.com/a/313990/5834961
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return str;
    }

    void loadWords()
    {
        std::string files[] = {"words_alpha.txt", "words.txt", "words_english.txt"};
        for (std::string filename : files)
        {
            std::ifstream file(filename);
            if (file.is_open())
            {
                std::string word;
                while (std::getline(file, word))
                {
                    word = toLower(word);
                    wordsTable.insert(std::pair<std::string, std::string>(word, word));
                }
            }
            file.close();
        }
        std::cout << wordsTable.size() << " words processed." << std::endl
                  << std::endl;
    }

    void parse(const std::string &text)
    {
        int start = 0;
        for (size_t i = 0; i < text.length(); ++i)
        {
            for (char delim : delimiters)
            {
                if (text[i] == delim)
                {
                    std::string sub = text.substr(start, i - start);
                    if (sub != "")
                        parsedText.push_back(sub);
                    start = i + 1;
                    break;
                }
            }
        }

        std::string remainder = text.substr(start);
        if (remainder != "")
            parsedText.push_back(remainder);
    }

    void search()
    {
        std::set<std::string> invalidWords;
        for (std::string word : parsedText)
        {
            if (wordsTable.find(toLower(word)) == wordsTable.end())
                invalidWords.emplace(word);
        }

        for (std::string invalidWord : invalidWords)
        {
            std::vector<std::string> possibleWordCombos = wordCombinations(toLower(invalidWord));
            if (!possibleWordCombos.empty())
            {
                std::cout << "In place of " << invalidWord << " did you mean ";
                if (possibleWordCombos.size() == 1)
                    std::cout << possibleWordCombos.at(0) << "?" << std::endl;
                else
                {
                    std::cout << "[";
                    std::string separator;
                    for (auto &x : possibleWordCombos)
                    {
                        std::cout << separator << x;
                        separator = ", ";
                    }
                    std::cout << "]" << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }

public:
    Autocorrect()
    {
        for (int i = 0; i < 26; ++i)
            alphabets[i] = 'a' + i;

        loadWords();
    }

    void suggest(const std::string &input)
    {
        parsedText.clear();
        parse(input);
        search();
    }

    std::vector<std::string> wordCombinations(std::string word)
    {
        std::vector<std::string> possibleWords;
        for (size_t i = 0; i < word.length(); ++i)
        {
            std::string start = word.substr(0, i);
            std::string end = word.substr(i + 1);

            for (char alphabet : alphabets)
            {
                std::string combiantion = start + alphabet + end;
                if (wordsTable.find(combiantion) != wordsTable.end())
                    possibleWords.push_back(combiantion);
            }
        }
        return possibleWords;
    }
};