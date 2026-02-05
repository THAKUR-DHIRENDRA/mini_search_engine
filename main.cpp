#include<stdio.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<filesystem>

using namespace std;
namespace fs = std::filesystem;

bool keyfinder(string line, string key) 
{
    int p1 = 0, p2 = 0;
    while (p1 < line.size()) 
    {
        if (line[p1] == key[p2])
            p1++, p2++;
        else 
        {
            // Logic fix: reset p1 to the next char after the start of the previous                   match attempt
            p1 = p1 - p2 + 1; 
            p2 = 0;
        }
    }
    return p2 == key.size();
}

void filesfinder(string folderpath, vector<pair<string,int>>& result, string key) 
{
    // Standard check to ensure the folder exists
    if (!fs::exists(folderpath)) 
    {
        cout << "Folder not found: " << folderpath << endl;
        return;
    }

    //Results of your code will appear here when 

    for (const auto& file : fs::directory_iterator(folderpath)) 
    {
        // FIX: Pass the actual file path object, not a string literal in quotes
        ifstream reader(file.path()); 
        int count=0;
        string line;
        if (!reader.is_open()) 
        {
            // Use 'continue' instead of 'break' so it tries the next file if one fails
            cout << "File could not be opened: " << file.path().filename() << endl;
            continue; 
        }

        while (getline(reader, line)) 
        {
            if (keyfinder(line, key)) 
            {
                count++;
            }
        }

        reader.close(); // Good practice to close the file

        if (count) 
        {
            // Store the filename as a string
            result.push_back({file.path().filename().string(),count});
        }
    }



    //now sort the vector based on count in decreasing order
    sort(result.begin(), result.end(), [](const pair<string, int>& a, const pair<string, int>& b) 
    {
        return a.second > b.second; // Primary sort: highest frequency
    });
    
}

int main() 
{
    string key;
    cout << "Enter search key: ";
    cin >> key;

    vector <pair<string,int>> result;
    // Ensure this folder exists in your project directory
    string folderpath = "txt_files"; 

    filesfinder(folderpath,result,key);

    // Print the results so you know it worked!
    if (result.empty()) 
    {
        cout << "No matches found." << endl;
    } 
    else 
    {
        cout << "Key found in: " << endl;
        for (const auto& filename : result) 
        {
            cout << "- " << filename.first<<" "<<filename.second<<endl;
        }
    }

    return 0;
}