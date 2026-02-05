#include<stdio.h>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<filesystem>
#include<bits/stdc++.h>
using namespace std;
namespace fs = std::filesystem;


//function to clean the word
void wordcleaner(string &word)
{
    string cleaned="";
    for(char ch:word)
        {
            if(!ispunct(ch))
                cleaned+=ch;
        }
    word=cleaned;
}

//function to convert to lowercase
void tolowercase(string &word)
{
    transform(word.begin(),word.end(),word.begin(), ::tolower);
}

void mapbuilder(string folderpath,unordered_map<string,unordered_map<string,int>>&invertedindex) 
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
        string line;
        while (getline(reader, line)) 
        {
           string word;
            stringstream ss(line);
            while(ss>>word)
                {
                    wordcleaner(word);
                    tolowercase(word);
                    if(!word.empty())
                    invertedindex[word][file.path().filename().string()]++;
                }
        }

        reader.close(); // Good practice to close the file
    }
}

int main() 
{
    string key;
    cout << "Enter search key: ";
    cin >> key;
    unordered_map<string,unordered_map<string,int>>invertedindex;
    // Ensure this folder exists in your project directory
    string folderpath = "txt_files"; 
    mapbuilder(folderpath,invertedindex);
    if(invertedindex.size()==0)
        cout<<"The files did not contain any texts\n";
    else
    {
        auto it=invertedindex.find(key);
        if(it!=invertedindex.end())
        {
            cout<<"Here are all the file tha contains the word: "<<key<<endl;
            for(const auto& fileEntry:it->second)
                {
                    cout<<" Filename-> "<<fileEntry.first<<" Frequencies : "<<fileEntry.second<<endl;
                }
        }
        else
        {
            cout<<"There are no files containing this word!!"<<endl;
        }
    }     
    return 0;
}