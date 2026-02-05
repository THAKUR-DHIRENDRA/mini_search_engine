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

class SearchEngine
{
private:
    // Private members: Only the class can touch these
    unordered_map<string, unordered_map<string, int>> invertedindex;
    unordered_map<string, int> doclengths;

    // Internal helper functions
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

    void tolowercase(string &word)
    {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
    }

public:
    // Constructor: Automatically builds the index when you create the object
    SearchEngine(string folderpath)
    {
        for (const auto &file : fs::directory_iterator(folderpath))
        {
            ifstream reader(file.path());
            if (!reader.is_open())
                continue;

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
            doclengths[filename] = count;
            reader.close();
        }
    }

    // Public search method: The "face" of your engine
    vector<pair<string, double>> search(string key)
    {
        vector<pair<string, double>> ranks;
        tolowercase(key); // Normalize user input

        auto it = invertedindex.find(key);
        if (it != invertedindex.end())
        {
            for (const auto &fileEntry : it->second)
            {
                auto dt = doclengths.find(fileEntry.first);
                if (dt != doclengths.end())
                {
                    double totalword = dt->second;
                    double totalfiles = doclengths.size();
                    double relatedfiles = it->second.size();
                    double frequency = fileEntry.second;

                    double TF = frequency / totalword;
                    double IDF = log10(totalfiles / relatedfiles);
                    ranks.push_back({fileEntry.first, TF * IDF});
                }
            }
            // Sort by rank before returning
            sort(ranks.begin(), ranks.end(), [](const auto &a, const auto &b) {
                return a.second > b.second;
            });
        }
        return ranks;
    }
};

int main()
{
    // 1. Initialize the engine (automatically runs mapbuilder)
    SearchEngine myEngine("txt_files");

    string query;
    cout << "Enter search key: ";
    cin >> query;

    // 2. Use the engine to get ranked results
    auto results = myEngine.search(query);

    // 3. Display the results
    if (!results.empty())
    {
        int rank = 1;
        for (const auto &res : results)
        {
            cout << "Rank " << rank++ << ": " << res.first << " (Score: " << res.second << ")" << endl;
        }
    }
    else
    {
        cout << "No results found for '" << query << "'" << endl;
    }

    return 0;
}