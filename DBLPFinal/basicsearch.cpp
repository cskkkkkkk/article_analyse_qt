#pragma once
#include "basicsearch.h"


namespace BasicSrarch {
inline string Hash4(const string& tar) {
    DWORD64 ans = 0;
    for (int i = 0; i < tar.length(); i++) {
        ans = ((ans >> 8) & 0xf) ^ ((ans << 4) ^ tar[i]);
    }
    ans = ans & 0xfff;
    return to_string(ans);
}

//将string类型转换为char*字符串数组的类型
void Strcpy(char* pt, const string& p2)
{
    strcpy_s(pt, p2.length() + 1, p2.c_str());
}

//把char*的常亮转换为char*数组类型
void Strcpy(char* pt, const char* p2)
{
    string temp = p2;
    strcpy_s(pt, temp.length() + 1, temp.c_str());
}



//无符号类型,将string转换为一个数字将'1314'字符串转化为数字1314
size_t stringToLonglong(string str)
{
    size_t ret = 0;
    for (auto i : str)
        ret = ret * 10 + i - '0';
    return ret;
}



bool check_TailElem(string tempstr)//尾节点判定
{
    if ((tempstr.find("</article>") != string::npos) || (tempstr.find("</inproceedings>") != string::npos) ||
        (tempstr.find("</proceedings>") != string::npos) || (tempstr.find("</book>") != string::npos) ||
        (tempstr.find("</incollection>") != string::npos) || (tempstr.find("</phdthesis>") != string::npos) ||
        (tempstr.find("</mastersthesis>") != string::npos) || (tempstr.find("</www>") != string::npos))
        return false;
    return true;
};

/// 输入文章名，返回文章的所有相关信息
/// <param name="_articleName">文章名</param>
/// <param name="_saveUrl">文件保存路径</param>
/// <param name="pt">对外输出结果</param>
void articleInfo(string _articleName, string _saveUrl, string&pt)
{
    string articleName = _articleName;
    string saveUrl = _saveUrl;
    string ret = ""; //返回值
    string flag = Hash4(articleName);//文件位置
    ifstream infile(saveUrl + "database\\article\\" + flag + ".ini", ios::in); //打开对应文档

    //路径错误，返回报错提示
    if (!infile){
        //Strcpy(pt, "");
        return;
    }

    //找到哈希文件中文章对应的偏移量
    size_t urlpt = 0;
    string tempstr;
    getline(infile, tempstr);

    while (!infile.eof()) {
        //if (tempstr.find(articleName + " <-> ") != string::npos)//Bug修复点
        if (tempstr.substr(0, articleName.length() + 5) == articleName + " <-> ")
        {
            size_t startPos = articleName.length() + 5;//获取位置后+1
            size_t endPos = tempstr.size() - 1;//最后的位置
            string d = tempstr.substr(startPos, endPos);
            urlpt = stringToLonglong(d);
            break;
        }
        getline(infile, tempstr);
    }
    infile.close();

    if (urlpt == 0)
    {
        //Strcpy(pt, "");
        return;
    }

    //打开dblp文件，通过偏移量移动指针，然后返回该书的所有内容
    int tot = 0;//表示现在读取的是第几行
    if (urlpt != 0)
    {
        ifstream indblp(saveUrl + "dblp.xml", ios::in | ios::binary);
        indblp.seekg(urlpt, ios::beg);
        //修正偏移量
        char tmp = indblp.get();
        while (tmp != '<') {
            //cout << tmp<<" "<< indblp.tellg()<<endl;
            indblp.seekg(--urlpt);
            tmp = indblp.get();
        }
        indblp.seekg(urlpt, ios::beg);
        getline(indblp, tempstr);



        //直到找到文章结尾才跳出循环
        while ((check_TailElem(tempstr) || tot <= 3) && !indblp.eof())//暴力输出3-1000行
        {

            //太短的跳过这个信息
            if (!check_TailElem(tempstr) && tempstr.length() <= 17) {
                tot++;
                if (tot > 1000) //防止特例造成死循环
                    break;
                getline(indblp, tempstr);
                continue;
            }
            ret += tempstr + "\n";
            tot++;
            if (tot > 1000) //防止特例造成死循环
                break;
            getline(indblp, tempstr);
        }
        //ret += tempstr + "\n";
    }
    //将结果与c#对接
    //一定要使用3个参数的strcpy_s，不然会报错（strcpy也报错，傻逼vs）
    //原理https://blog.csdn.net/leowinbow/article/details/82380252
    if (ret.length() > 0)
    {
       pt=ret; //拷贝数据输出
    }
    else
    {
        //Strcpy(pt, "");
        pt="";
    }
    return;
}


/// 输入作者名字，返回作者发布的所有文章
/// <param name="searchName">作者名字</param>
/// <param name="_saveUrl">文件保存路径</param>
/// <param name="pt">输出结果</param>
//先通过查找那个作者的文章名字，再把文章名字去搜索文章具体信息
void main_Reader(string searchName, string _saveUrl, string& pt)
{
    //Strcpy(pt, "");
    string search_name = searchName;
    string saveUrl = _saveUrl;
    string ret = ""; //返回值
    string flag = Hash4(search_name);
    string tempstr;

    ifstream infile(saveUrl + "database\\author\\" + flag + ".ini", ios::in);
    getline(infile, tempstr);

    if (!infile) {
        //Strcpy(pt, "");
        return;
    }

    while (!infile.eof()) {
        if (tempstr.find("$" + search_name + "$") != string::npos)
        {
            size_t startPos = tempstr.find_last_of("$") + 1;//获取位置后+1
            size_t endPos = tempstr.size() - 1;//最后的位置
            ret += tempstr.substr(startPos, endPos - startPos + 1) + "\n";
        }
        getline(infile, tempstr);
    }
    infile.close();
    if (ret.length() > 0){
        //Strcpy(pt, ret);
        pt=ret;
    }
    return;
}

LPCWSTR stringtolstr(string str) {
    LPWSTR ans = new WCHAR[str.length() + 1];
    MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, ans, str.length());
    return ans;
}


// 检查文件路径是否正确
bool CheckFilePathCorrect_Compatible(char* _saveUrl)
{
    ifstream infile((string)_saveUrl + "database\\finish.db", ios::in);
    //路径错误，返回报错
    if (!infile)
    {
        return false;
    }
    ifstream indblp((string)_saveUrl + "dblp.xml", ios::in);
    if (!indblp)
    {
        return false;
    }
    return true;
}

// 检查文件路径是否正确


bool CheckFilePathCorrect(char* _saveUrl)
{
    string saveUrl = _saveUrl;
    HANDLE check_file_handle = CreateFile(stringtolstr(saveUrl + "database\\finish.db"), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (check_file_handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    HANDLE check_file_handle2 = CreateFile(stringtolstr(saveUrl + "dblp.xml"), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (check_file_handle2 == INVALID_HANDLE_VALUE)
        return false;
    return true;
    //ifstream infile(saveUrl + "database\\finish.db", ios::in);
    ////路径错误，返回报错
    //if (!infile)
    //{
    //    return false;
    //}
    //ifstream indblp(saveUrl + "dblp.xml", ios::in | ios::binary);
    //if (!indblp)
    //{
    //    return false;
    //}
}

class set_coauthor {
private:
    set<string> coauthor_all;
    int total;
public:
    set_coauthor() {
        coauthor_all.clear();
        total = 0;
    }
    ~set_coauthor(){}
    void put(string tar) {
        string tmp = "";
        string::iterator t = tar.begin();
        while (t != tar.end()) {
            if (*t == '$' && tmp != "") {
                coauthor_all.insert(tmp);
                tmp = "";
                total++;
            }
            else if (*t != '$') {
                tmp = tmp + *t;
            }
            t++;
        }
    }
    void clear() {
        coauthor_all.clear();
        total = 0;
    }
    string get(string himself = "") {
        string ans = "";
        set<string>::iterator t = coauthor_all.begin();
        while (t != coauthor_all.end()) {
            if (*t != himself)
                ans += *t + "\n";
            t++;
        }
        return ans;
    }
};


void CoAuthor(string searchName, string _saveUrl, string &pt)
{
    //Strcpy(pt, "");
    string search_name = searchName;
    string saveUrl = _saveUrl;
    string ret = ""; //返回值
    string flag = Hash4(search_name);
    string tempstr;

    ifstream infile(saveUrl + "database\\author\\" + flag + ".ini", ios::in);
    getline(infile, tempstr);

    if (!infile) {
       // Strcpy(pt, "");
        return;
    }
    set_coauthor tmps;
    qDebug()<<"m1";
    while (!infile.eof()) {
        if (tempstr.find("$" + search_name + "$") != string::npos)
        {
            tmps.put(tempstr);
        }
        getline(infile, tempstr);
    }
    qDebug()<<"m2";
    infile.close();
    ret = tmps.get(search_name);
    if (ret.length() > 0) {
        //Strcpy(pt, ret);
        pt=ret;
    }
    return;
}
}
