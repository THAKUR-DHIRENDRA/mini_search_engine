#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <cmath> 
#include <string>

using namespace std;
namespace fs = std::filesystem;

//function to clean the word
void wordcleaner(string &word)
{
    string cleaned = "";
    for (char ch : word)
    {
        if (!ispunct(ch))
            cleaned += ch;
    }
    word = cleaned;
}

//function to convert to lowercase
void tolowercase(string &word)
{
    transform(word.begin(), word.end(), word.begin(), ::tolower);
}

void mapbuilder(string folderpath, unordered_map<string, unordered_map<string, int>>& invertedindex, unordered_map<string, int>& doclengths)
{
    for (const auto& file : fs::directory_iterator(folderpath))
    {
        // FIX: Pass the actual file path object, not a string literal in quotes
        ifstream reader(file.path());
        if (!reader.is_open())
        {
            // Use 'continue' instead of 'break' so it tries the next file if one fails
            cout << "File could not be opened: " << file.path().filename() << endl;
            continue;
        }
        string filename = file.path().filename().string();
        string line;
        int count = 0;
        while (getline(reader, line))
        {
            string word;
            stringstream ss(line);
            while (ss >> word)
            {
                count++;
                wordcleaner(word);
                tolowercase(word);
                if (!word.empty())
                    invertedindex[word][filename]++;
            }
        }
        doclengths[filename] = count; // Good practice to close the file
        reader.close(); // Good practice to close the file
    }
}

void rankgenerator(vector<pair<string, double>>& ranks, string key, unordered_map<string, unordered_map<string, int>>& invertedindex, unordered_map<string, int>& doclengths)
{
    //no of words in each file
    //2nd no of files containing that keyword
    //frequency of that keyword
    //files with that keyword
    //tf=frequency/total words in that file
    //idf=log(total no of files/no of files containing that word)
    double TF = 0, IDF = 0;
    auto it = invertedindex.find(key);
    if (it != invertedindex.end())
    {
        for (const auto& fileEntry : it->second)
        {
            auto dt = doclengths.find(fileEntry.first);
            if (dt != doclengths.end())
            {
                double totalword = dt->second; 
                double totalfiles = doclengths.size();
                double relatedfiles = it->second.size();
                double frequency = fileEntry.second; 

                TF = frequency / totalword;
                IDF = log10(totalfiles / relatedfiles);
                double result = TF * IDF;
                ranks.push_back({fileEntry.first, result});
            }
        }
    }
}

int main()
{
    //inverted index will store key word , file name associated with , frequency of keyword
    unordered_map<string, unordered_map<string, int>> invertedindex;

    //doclengths will store file name, no of words in that file
    unordered_map<string, int> doclengths;

    vector<pair<string, double>> ranks;

    // Ensure this folder exists in your project directory
    string folderpath = "txt_files";
    mapbuilder(folderpath, invertedindex, doclengths);

    string key;
    cout << "Enter search key: ";
    cin >> key;
    tolowercase(key); 

    rankgenerator(ranks, key, invertedindex, doclengths);

    // Sorting the results to present the highest rank first
    sort(ranks.begin(), ranks.end(), [](const auto& a, const auto& b)
    {
        return a.second > b.second;
    });

    if (!ranks.empty())
    {
        int count=1;
        for (const auto& it : ranks)
        {
            cout << "File name: " << it.first << " rank " << count<< endl;
            count++;
        }
    }
    else
    {
        cout << "There are no files that contain this keyword" << endl;
    }

    return 0;
}