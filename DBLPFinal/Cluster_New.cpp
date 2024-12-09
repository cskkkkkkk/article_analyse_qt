#pragma once
#include<iostream>
#include<vector>
#include<map>
#include "AllHeader.h"
using namespace std;
int perSum = 256;
int num = 16;//线程数
set<int>ss[16];
bool isin[66000] = { false };
char File[] = "C:\\Users\\86183\\Desktop\\DblpFolder\\";
int activeThread = 0;//活跃线程数量


set<int>R;
set<int>P;
set<int>X;
//统计各阶完全子图数量的数据结构
map<int, int>ans;



struct Edge
{
    int vName;
    struct Edge* next;
};
struct Vertex
{
    int vName;
    struct Edge* next;
};
class Graph
{
public:
    Graph()
    {
        vVertex = new Vertex[66000];
        for (int i = 0; i < 66000; i++)
        {
            vVertex[i].vName = i;
            vVertex->next = NULL;
        }
    }
    void graphInit();
    void insertEdge(int from, int to);
    Vertex* vVertex;


};

void Graph::insertEdge(int from, int to)
{
    Edge* edge = new Edge();
    edge->vName = to;
    edge->next = NULL;
    if (vVertex[from].next != NULL)
    {
        Edge* tmp = vVertex[from].next;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = edge;
    }
    else
    {
        vVertex[from].next = edge;
    }
}
Graph graph;


inline int Hash(string tar)
{
    int total = 0;
    for (int i = 0; i < tar.size(); i++)
    {
        total += (int)tar[i];
    }
    return (total % 66000);
}

inline int pos(string tar)
{
    int num = 0;//探测了几次
    int h = Hash(tar);
    while (isin[h])
    {
        num++;
        h += num * num;
    }
    return h % 66000;
}

inline int Hash4(const string& tar) {
    DWORD64 ans = 0;
    for (int i = 0; i < tar.length(); i++) {
        ans = ((ans >> 8) & 0xf) ^ ((ans << 4) ^ tar[i]);
    }
    ans = ans & 0xfff;
    return ans;
}

void createGraph(vector<int>& tmp)
{
    for (int i = 0; i < tmp.size(); i++)
    {
        for (int j = 0; j < tmp.size(); j++)
        {
            if (tmp[i] == tmp[j])
                continue;
            graph.insertEdge(i, j);
        }
    }



}

void put(string tar)
{
    string tmp = "";
    string::iterator t = tar.begin();
    vector<int>temp;
    while (t != tar.end())
    {
        if (*t == '$' && tmp != "")
        {
            int ha = pos(tmp);
            isin[ha] = true;
            temp.push_back(ha);
            P.insert(ha);
            tmp = "";
        }
        else if (*t != '$') {
            tmp = tmp + *t;
        }
        t++;
    }
    //结束后用vector tmp处理
    //建图处理
    createGraph(temp);
    temp.clear();
}

inline void read_create(fstream& iner, int start, int end, int id)
{
    string ret = ""; //返回值

    for (int i = start; i < end; i++)
    {
        string tempstr = "";

        ifstream infile((string)File + "database\\author\\" + to_string(i) + ".ini", ios::in);

        if (!infile) {
            cout << "打开文件" << i << ".ini" << "失败" << endl;
            return;
        }
        getline(infile, tempstr);
        while (!infile.eof())
        {
            put(tempstr);
            getline(infile, tempstr);
        }

        activeThread--;
        infile.close();
    }



}

void reader(char* _FileUrl, Graph& graph)
{
    thread* read[16];
    fstream* infile = new fstream[16];
    /*
    for (int i = 0; i < 16; i++) {
        infile[i].open((string)_FileUrl + "dblp.xml", ios::in);
        if (!infile[i]) {
            cout << "文件流" << i << "未能打开文件" << (string)_FileUrl +to_string(i)<< endl;
            return;
        }
    }
      */
    for (int i = 0; i < 16; i++)
    {
        (read[i]) = new thread(read_create, ref(infile[i]), i * perSum, (i + 1) * perSum - 1, i);
        activeThread++;
        while (activeThread >= 16)
        {
            cout << "读取中。。。" << endl;
        }
    }
    while (activeThread != 0)
    {
        cout << "目前线程数量为:" << activeThread << endl;
    }
    cout << "读入完毕" << endl;
    int total = 0;
    for (int i = 0; i < 16; i++)
    {
        total += ss[i].size();
    }
    cout << "节点总数量为:" << total << endl;

}


//实现交运算
set<int> jiao(int item, set<int>tmp, Graph& graph)
{
    set<int>tm;
    set<int>result;
    Edge* edge = graph.vVertex[item].next;
    if (edge == NULL)
    {

    }
    else
    {
        tm.insert(edge->vName);
        while (edge->next != NULL)
        {
            edge = edge->next;
            tm.insert(edge->vName);
        }

    }

    set_intersection(tmp.begin(), tmp.end(), tm.begin(), tm.end(), inserter(result, result.begin()));
    return result;
}


set<int> bing(int item, set<int> tmp)
{
    set<int>tm;
    tm.insert(item);
    set<int>result;
    set_union(tmp.begin(), tmp.end(), tm.begin(), tm.end(), inserter(result, result.begin()));
    return result;
}

set<int> dele(int item, set<int>tmp)
{
    set<int>tm;
    tm.insert(item);
    set<int>result;
    set_difference(tmp.begin(), tmp.end(), tm.begin(), tm.end(), inserter(result, result.begin()));
    return result;
}

//存储数据
void store(int size)
{
    auto pos = ans.find(size);
    if (pos != ans.end())
    {
        pos->second++;
    }
    else
    {
        ans.insert(pair<int, int>(size, 1));
    }
}


//实现算法
void BronKerbosch(set<int>R, set<int>P, set<int>X, Graph& graph)
{
    if (P.size() == 0)
    {
        //把答案放进map里面
        store(P.size());

        //存储完毕就返回吧
        return;
    }
    for (auto item : P)
    {
        R.insert(item);
        P = jiao(item, P, graph);
        X = jiao(item, X, graph);
        BronKerbosch(R, P, X, graph);
        P = dele(item, P);
        X = bing(item, X);
    }

}

void write()
{
    fstream outfile((string)File + "database\\CoAuthorGraph_result", ios::out);
    for (auto item : ans)
    {

        cout << "阶数为:" << item.first << " " << "子图个数为:" << item.second << endl;
        outfile << "$" << item.first << "$" << item.second << endl;
    }
    outfile.close();

}

void run(char* _FileUrl)
{



    cout << "图初始化完毕!" << endl;
    reader(_FileUrl, graph);
    cout << "P初始化数量为:" << P.size() << endl;
    BronKerbosch(R, P, X, graph);
    //输出结果
    write();

}


/*
#include <iostream>
#include <vector>
#include <set>

using namespace std;

// 根据顶点的度数对顶点进行排序
bool compareDegrees(int v1, int v2, const vector<int>& degrees) {
    return degrees[v1] > degrees[v2];
}

// 计算每个顶点的度数
vector<int> computeDegrees(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> degrees(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            degrees[i] += graph[i][j];
        }
    }
    return degrees;
}

// 递归函数，找到所有的最大团
void bronKerbosch(vector<vector<int>>& graph, set<int> R, set<int> P, set<int> X,
                  vector<set<int>>& maxCliques, vector<pair<int, int>>& cliqueInfo) {
    if (P.empty() && X.empty()) {
        // R 是一个最大团
        maxCliques.push_back(R);

        // 计算该团的阶数
        int order = R.size();
        cliqueInfo.push_back(make_pair(order, 1)); // (阶数，个数)
    }

    // 对 P 中的顶点根据度数进行排序
    vector<int> degrees = computeDegrees(graph);
    vector<int> sortedP(P.begin(), P.end());
    sort(sortedP.begin(), sortedP.end(), [&](int v1, int v2) {
        return compareDegrees(v1, v2, degrees);
    });

    // 顶点邻接性检查
    for (auto u : sortedP) {
        if (graph[u].size() < P.size() || graph[u].size() < X.size()) {
            continue; // 剪枝
        }
        bool adjacent_to_all = true;
        for (auto v : R) {
            if (graph[u][v] != 1) {
                adjacent_to_all = false;
                break;
            }
        }
        if (!adjacent_to_all) {
            continue; // 剪枝
        }

        // 递归调用
        set<int> newR = R;
        newR.insert(u);
        set<int> newP, newX;
        for (auto vertex : P) {
            if (graph[u][vertex] == 1) newP.insert(vertex);
        }
        for (auto vertex : X) {
            if (graph[u][vertex] == 1) newX.insert(vertex);
        }
        bronKerbosch(graph, newR, newP, newX, maxCliques, cliqueInfo);
        P.erase(u);
        X.insert(u);
    }
}

// 寻找最大团
vector<set<int>> findMaxCliques(vector<vector<int>>& graph, vector<pair<int, int>>& cliqueInfo) {
    vector<set<int>> maxCliques;
    set<int> R, P, X;
    int n = graph.size();

    // 初始化P为所有顶点
    for (int i = 0; i < n; ++i) {
        P.insert(i);
    }

    bronKerbosch(graph, R, P, X, maxCliques, cliqueInfo);

    return maxCliques;
}

int main() {
    // 举例：有向图的邻接矩阵
    vector<vector<int>> graph = {
        {0, 1, 1, 0, 1},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {0, 1, 1, 0, 1},
        {1, 0, 1, 1, 0}
    };

    vector<pair<int, int>> cliqueInfo;
    vector<set<int>> maxCliques = findMaxCliques(graph, cliqueInfo);

    // 输出所有最大团
    cout << "所有最大团：" << endl;
    for (int i = 0; i < maxCliques.size(); ++i) {
        cout << "团" << i + 1 << ": ";
        for (auto vertex : maxCliques[i]) {
            cout << vertex << " ";
        }
        cout << endl;
    }

    // 输出各阶完全子图的个数及阶数
    cout << "各阶完全子图的个数及阶数：" << endl;
    for (int i = 0; i < cliqueInfo.size(); ++i) {
        cout << "阶数 " << cliqueInfo[i].first << " 的完全子图个数：" << cliqueInfo[i].second << endl;
    }

    return 0;
}

*/

