
#pragma once
#include "AllHeader.h"
namespace Fuzzy_Search {
using namespace std;
void calculate_thread(char*, DWORD, DWORD, DWORD);
inline string Hash4( string& tar);
//int Full_word_match_fuzzy_search(char*, DWORD, string,vector<string>&);
int Full_word_match_fuzzy_search(char*, DWORD, string,string&);
//int fuzzy_search(char*, DWORD, char*,vector<string>&);
int fuzzy_search(char*, DWORD, char*,string&);
void initial_fuzzy(char*);
void release_fuzzy_searcher();
inline DWORD Format_Hash4(char*);
}
