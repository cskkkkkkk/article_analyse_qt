#pragma once
#include "AllHeader.h"

namespace AFauthorSorting {
    using namespace std;
    class node;
    class authors;
    bool Author_Sort(char*);
    inline LPCWSTR stringtolstr(string);
    inline void Build_Tree(fstream&, node*&);
    void Insert_Tree(node*, string, DWORD);
    void Found_Tree(node*&, vector<authors>&, string );
    inline void sort(vector<authors>&, DWORD, DWORD);
    template<typename T> inline void quick_sort(vector<T>&, DWORD, DWORD);
    template<typename T> inline void bubble_sort(vector<T>&, DWORD, DWORD);
}

namespace Keyword_Sorting {
    using namespace std;
    void Keyword_Sort(string year, string);
    inline void Build_Trees(fstream&, AFauthorSorting::node*&);
}
