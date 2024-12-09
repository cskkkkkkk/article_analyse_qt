#pragma once
#ifndef BASICSEARCH_H
#define BASICSEARCH_H
#include"AllHeader.h"

namespace BasicSrarch {
using namespace std;
inline string Hash4(const string&);
inline void Strcpy(char*, const string&);
inline size_t stringToLonglong(string);
inline void Strcpy(char*, const char*);
inline LPCWSTR stringtolstr(string);
inline bool check_TailElem(string);
class set_coauthor;
void articleInfo(string, string, string&);
void main_Reader(string, string, string&);
bool CheckFilePathCorrect_Compatible(char*);
bool CheckFilePathCorrect(char*);
void CoAuthor(string, string, string&);
}

#endif // BASICSEARCH_H
