# Mini Search Engine (C++)

## Overview
A C++ search engine that uses an inverted index with TF-IDF scoring to rank text documents by relevance. It supports incremental indexing via file timestamp tracking.

## Project Structure
- `main.cpp` - Main source file containing SearchEngine and EngineTester classes
- `txt_files/` - Directory of text documents (doc1.txt through doc10.txt)
- `index.txt` - Serialized inverted index (auto-generated)

## How to Run
Compile with C++17: `g++ -std=c++17 -o main main.cpp && ./main`

## Key Components
- **SearchEngine**: Builds/loads inverted index, syncs with folder, searches using TF-IDF
- **EngineTester**: Runs accuracy, edge case, performance, and stress tests
- **Index Persistence**: Saves/loads index to `index.txt` with doc data and timestamps

## Recent Changes
- 2026-02-19: Created missing `doc1.txt` with algorithm-focused content
- 2026-02-19: Adjusted `doc5.txt` and `doc8.txt` to fix TF-IDF ranking for "software" query
- 2026-02-19: Deleted stale `index.txt` that had orphaned doc1.txt references without timestamps
