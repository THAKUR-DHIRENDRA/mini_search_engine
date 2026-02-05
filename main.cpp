#include<stdio.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<filesystem>

using namespace std;
namespace fs = std::filesystem;

int keyfinder(string line, string key) {
    int count = 0;
    // Start searching from the beginning (index 0)
    size_t pos = line.find(key, 0); 

    while (pos != string::npos) {
        count++;
        // Move the 'pos' to the index right after the current match
        // This prevents the loop from finding the same word forever
        pos = line.find(key, pos + key.length());
    }
    return count;
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
            count+= keyfinder(line, key);
        }

        reader.close(); // Good practice to close the file

        if (count) 
        {
            // Store the filename and its count
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