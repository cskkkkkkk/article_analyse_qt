#ifndef DATA_H
#define DATA_H

#include"AllHeader.h"
namespace data {
    using namespace std;
    inline DWORD Hash4(const string&);//12位字符串哈希
    bool initial_readers(DWORD, DWORD, bool,string);//线程参数配置
    void reader(fstream&, DWORD, DWORD, DWORD);//数据处理核心线程
    inline DWORD Write_Record(fstream&, DWORD, string&, DWORD);
    inline void Create_article(string, DWORD, vector<string>&, DWORD, DWORD);
    inline void do_writer();//文件输出函数
    void writer();//数据处理输出线程
    inline void push_wz(string, DWORD);
    inline void push_year(string, DWORD);
    inline void push_author(string, DWORD);
    inline bool gotchar(fstream&, DWORD, char&);
    inline LPCWSTR stringtolstr(string);
    template<typename T> inline void sort(vector<T>&, int, int);
    class out_string; //输出内容缓存
}

#endif // DATA_H
