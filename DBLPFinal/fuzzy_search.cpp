#include "fuzzy_search.h"
namespace Fuzzy_Search {
bool change_fuzzy_string[257];//用于指示每个线程是否需要改变模糊搜索的目标字符串。索引对应线程的ID。当某个线程的
//change_fuzzy_string[ID] 被设置为 true 时，表示需要改变模糊搜索的目标字符串。
string fuzzy_target = "";//存储当前的模糊搜索目标
bool kill_me = false;//，用于指示是否需要停止模糊搜索。
vector<string> bags;//一个字符串向量，存储了待搜索的字符串数据集合。
bool finished_flag[257];//用于指示每个线程的模糊搜索任务是否已完成。索引对应线程的ID。
vector<string> answers[257];//用于存储每个线程的模糊搜索结果。索引对应线程的ID
int max_thread_num;//表示最大线程数量
bool is_initial = false;//用于指示模糊搜索器是否已经初始化


/*
_saveUrl：保存文件的路径。
lowbound：搜索范围的下界。
upbound：搜索范围的上界。
ID：线程的标识符。
*/
void calculate_thread(char* _saveUrl, DWORD lowbound, DWORD upbound, DWORD ID)
{
    if (lowbound >= upbound)return;
    while (!kill_me) {
        if (fuzzy_target == "") {
            Sleep(100);
            continue;
        }
        if (!change_fuzzy_string[ID]) {
            Sleep(100);
            continue;
        }
        change_fuzzy_string[ID] = false;
        finished_flag[ID] = false;
        string ans = "";
        int* kmp = new int[fuzzy_target.length() + 10];
        kmp[0] = 0;
        int k1 = 0, j1 = 1;
        //相当于初始化kmp算法中的next数组
        while (j1 < fuzzy_target.length()) {
            if (fuzzy_target[k1] == fuzzy_target[j1]) {
                k1++;
                kmp[j1] = k1;
                j1++;
            }
            else if (k1 != 0) {
                k1 = kmp[k1];
            }
            else {
                kmp[j1] = 0;
                j1++;
            }
        }
        for (int i = lowbound; i < upbound; i++) {
            k1 = 0, j1 = 0;
            while (j1 < bags[i].length()) {
                if (k1 >= fuzzy_target.length())break;
                else if (fuzzy_target[k1] == bags[i][j1]) {
                    k1++; j1++;
                }
                else if (k1 != 0) {
                    k1 = kmp[k1];
                }
                else {
                    j1++;
                }
            }
            if (k1 >= fuzzy_target.length()) {
                answers[ID].push_back(bags[i]);
            }
        }
        finished_flag[ID] = true;
    }
    return;
}


//Hash4函数
inline string Hash4( string& tar) {
    DWORD64 ans = 0;
    for (int i = 0; i < tar.length(); i++) {
        ans = ((ans >> 8) & 0xf) ^ ((ans << 4) ^ tar[i]);
    }
    ans = ans & 0xfff;
    return to_string(ans);
}



//接受数据的格式不相同
inline DWORD Format_Hash4(char* tars) {
    string tar = tars;
    DWORD64 ans = 0;
    while (tar[0] == ' ') {
        tar.erase(0, 1);
    }
    while (tar[tar.size() - 1] == ' ') {
        tar.erase(tar.size() - 1, 1);
    }
    tar = " " + tar + " ";
    for (int i = 0; i < tar.length(); i++) {
        ans = ((ans >> 8) & 0xf) ^ ((ans << 4) ^ tar[i]);
    }
    ans = ans & 0xfff;
    return ans;
}


//预处理读进来的tmp
int Full_word_match_fuzzy_search(char* _saveUrl, DWORD thread_num, string tar,string&num)
{
    string tmp = tar;
    while (tmp[0] == ' ') {
        tmp.erase(0, 1);
    }
    while (tmp[tmp.size() - 1] == ' ') {
        tmp.erase(tmp.size() - 1, 1);
    }
    tmp = " " + tmp + " ";
    char* tmps = new char[tmp.size() + 1];
    for (int i = 0; i < tmp.size(); i++) {
        tmps[i] = tmp[i];
    }
    tmps[tmp.size()] = '\0';
    qDebug()<<"full";
    return fuzzy_search(_saveUrl, thread_num, tmps,num);
}


//模糊查询指的是文章名字
void initial_fuzzy(char* _saveUrl)
{
    bags.clear();
    for (int i = 1973; i <= 2022; i++) {
        fstream infile((string)_saveUrl + "database\\year\\" + to_string(i) + ".ini", ios::in);
        if (!infile) {
            continue;
        }
        string instr;
        getline(infile, instr);
        while (!infile.eof()) {
            bags.push_back(instr);
            getline(infile, instr);
        }
    }
    is_initial = true;
    return;
}


//检查模糊搜索是否完成，kill_me是停止所有的线程，finish_flag是指每个线程都完成了各自的任务
bool check_finish() {
    bool ans = true;
    for (int i = 0; i < max_thread_num; i++) {
        ans = ans && finished_flag[i];
    }
    return (ans || kill_me);
}

int fuzzy_search(char* _saveUrl, DWORD thread_num, char*tar,string&num)
{
    if (!is_initial) {
        cout << "未初始化" << endl;
        return false;
    }
    if (thread_num > 256)thread_num = 256;
    max_thread_num = thread_num;
    kill_me = false;
    DWORD numb = bags.size() / thread_num;
    fuzzy_target = tar;
    for (int i = 0; i < max_thread_num; i++) {
        answers[i].clear();
        finished_flag[i] = false;
        change_fuzzy_string[i] = true;
    }
    qDebug()<<"A";
    for (int i = 0; i < thread_num - 1; i++) {
        thread* tmp = new thread(calculate_thread, _saveUrl, numb * i, numb * (i + 1), i);
        tmp->detach();
        //调用 tmp->detach() 将线程设置为分离状态，这意味着主线程与子线
        //程的执行相互独立，主线程不会等待子线程执行完毕而继续执行自己的任务。
    }
   qDebug()<<"B";
    thread* tmp = new thread(calculate_thread, _saveUrl, numb * (thread_num - 1), bags.size() - 1, thread_num - 1);
    tmp->detach();

     qDebug()<<"C";
    while (!check_finish()) { Sleep(2); }
     qDebug()<<"D";
     qDebug()<<"kill_me is:"<<kill_me;
    //如果强行停止就是false的状态未成功匹配
    if (kill_me)return false;
    kill_me = true;


    /*
    这部分是重定向输出，2 表示标准错误输出，nul
    是 Windows 中的特殊设备，表示空设备，重定向到 nul 就是将标准错误输出重定向到空设备
    ，即不输出错误信息。
    */
   qDebug()<<"E";
    string paths = "mkdir " + (string)_saveUrl + "database\\searchlog  2>nul";
    char* pathss = new char[paths.length() + 1];
    for (int i = 0; i < paths.length(); i++) {
        pathss[i] = paths[i];
    }
    pathss[paths.length()] = '\0';
    system(pathss);
    num=Hash4(fuzzy_target);
    fstream outfile((string)_saveUrl + (string)"database\\searchlog\\" + Hash4(fuzzy_target) + ".txt", ios::out);
    int tot = 0;
    for (int i = 0; i < thread_num; i++) {
        for (int j = 0; j < answers[i].size(); j++) {
            outfile << answers[i][j] << endl;
            //fin.push_back(answers[i][j]);
            tot++;
        }
        answers[i].clear();
    }
    outfile.close();
    //tot记录有多少个搜索记录，匹配的模糊查询结果
    return tot;
}


//将bags和answers中的内容全部给清除掉，然后设置一些标志位
void release_fuzzy_searcher()
{
    kill_me = true;
    bags.clear();
    for (int i = 0; i < 256; i++)answers[i].clear();
    is_initial = false;
    return;
}

}

