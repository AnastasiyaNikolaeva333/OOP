#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
//разбить на функции

const std::string COMMAND_STOP = "...";
const std::string NOT_SAVE = "";
const std::string FILD_DICTIONARY = "dictionary.txt";

void Error(const std::string& messege)
{
    std::cout << messege;
}

std::string EditeLine(const std::string& inputWord)
{
    std::istringstream iss(inputWord);
    std::vector<std::string> words;
    std::string word;

    while (iss >> word)
    {
        words.push_back(word);
    }

    std::string resultString;
    for (size_t i = 0; i < words.size(); ++i)
    {
        if (i > 0)
        {
            resultString += ' ';
        }
        resultString += words[i];
    }

    return resultString;
}

std::string ReadInputWord()
{
    std::string inputWord;
    if (std::getline(std::cin, inputWord))
    {
        inputWord = EditeLine(inputWord);
    }

    return inputWord;
}
bool IsFindWordInDiftionary(std::ifstream& diftionary, const std::string& word, std::string& russiaWord)
{
    diftionary.clear();
    diftionary.seekg(0, std::ios::beg);

    std::string line;
    bool found = false;

    while (std::getline(diftionary, line) && !found)
    {
        line = EditeLine(line);

        if (line.length() >= word.length())
        {
            int i = 0;
            while (i < word.size() && line[i] == word[i])
            {
                ++i;
            }

            if (i == word.size())
            {
                found = true;
                if (line.length() > word.length())
                {
                    russiaWord = line.substr(word.size() + 1);
                }
            }
        }
    }
    return found;
}

void StopDialog(std::map<std::string, std::string>& newWords, const std::string& fileDiftionary, bool flagEmpty)
{
    if (!newWords.empty())
    {
        std::cout << "В словарь были внесены изменения. Введите Y или y для сохранения перед выходом.\n";
        std::string answer;
        std::getline(std::cin, answer);
        answer = EditeLine(answer);
        if (answer == "Y" || answer == "y")
        {
            std::ofstream diftionary(fileDiftionary, std::ios::app);
            for (auto word : newWords)
            {

                if (!flagEmpty)
                {
                    diftionary << "\n";
                }
                diftionary << word.first << " " << word.second;
            }
            std::cout << "Изменения сохранены. До свидания.";
        }
        else
        {
            std::cout << "До свидания.";
        }
    }
    else
    {
        std::cout << "До свидания.";
    }
}

void FindInNewAddWord(std::map<std::string, std::string>& newWords, const std::string& word, std::string& russiaWord)
{
    if (newWords.find(word) != newWords.end())
    {
        russiaWord = newWords.find(word)->second;
        std::cout << russiaWord << "\n";

    }
    else
    {
        std::cout << "Неизвестное слово \"" << word << "\". Введите перевод или пустую строку для отказа.\n";
        std::getline(std::cin, russiaWord);
        russiaWord = EditeLine(russiaWord);
        if (russiaWord != NOT_SAVE)
        {
            newWords.insert({ word, russiaWord });
            std::cout << "Слово \"" << word << "\" сохранено в словаре как \"" << russiaWord << "\".\n";
        }
        else
        {
            std::cout << "Слово \"" << word << "\" проигнорировано.\n";
        }
    }
}

void WorkDiolog(const std::string& fileDiftionary)
{
    std::ifstream diftionary(fileDiftionary);
    if (!diftionary.is_open())
    {
        Error("Словарь нельзя открыть");
        return;
    }
    bool flagEmpty = diftionary.peek() == std::ifstream::traits_type::eof();
    bool flagEnd = false;
    std::map<std::string, std::string> newWords;
    std::string russiaWord;
    while (!flagEnd)
    {
        russiaWord = "";
        std::string word = ReadInputWord();
        if (word == COMMAND_STOP)
        {
            StopDialog(newWords, fileDiftionary, flagEmpty);
            flagEnd = true;
        }
        else if (!word.empty() && IsFindWordInDiftionary(diftionary, word, russiaWord))
        {
            if (russiaWord.empty())
            {
                Error("Отсутствует перевод слова\n");
            }
            else
            {
                std::cout << russiaWord << "\n";
            }
        }
        else if (!word.empty())
        {
            FindInNewAddWord(newWords, word, russiaWord);
        }
    }
    diftionary.close();
}