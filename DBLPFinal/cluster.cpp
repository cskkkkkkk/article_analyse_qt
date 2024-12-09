#include "cluster.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include<sstream>
using namespace std;

unordered_map<string, int> Discretization; // 作者名字数据离散化
int Parallel[3500000]; // 并查集
int authorIndex = 0; // 离散化编号的计数器

void ParallelNodeMerge(int& x, int y) {
    x = y;
}

int Findx(int x) { // 查找并查集父节点
    if (Parallel[x] != x)
        return Parallel[x] = Findx(Parallel[x]);
    return x;
}

int main() {
    // 并查集初始化
    for (int i = 0; i < 3500000; i++) {
        Parallel[i] = i;
    }

    for (int i = 0; i < 4096; i++)
    {
        cout << "正在处理第" << i << "个文件" << endl;

        ifstream infile("C:\\Users\\86183\\Desktop\\DblpFolder\\database\\author\\"+to_string(i)+".ini");
        string line;
        int titleAua = 0;
        while (getline(infile, line)) {

            titleAua = 0; // 重置序列号
            string tmp = "";
            string::iterator t = line.begin();
            while (t != line.end()) {
                if (*t == '$' && tmp != "") {
                    titleAua++; // 记录一次序列号
                    string authorName = tmp;
                    if (Discretization.find(authorName) == Discretization.end()) { // 如果该作者没有编号
                        Discretization[authorName] = authorIndex++; // 分配一个离散化编号
                    }
                    if (titleAua >= 2) { // 如果该作者不是该文章中的第一个作者
                        ParallelNodeMerge(Parallel[Discretization[authorName]], Parallel[Discretization[authorName] - 1]); // 与前一个作者节点合并
                    }
                    tmp = "";

                }
                else if (*t != '$') {
                    tmp = tmp + *t;
                }
                t++;
            }




        }
        infile.close();
        cout << i << endl;
    }


    // 统计聚团并保存结果
    unordered_map<int, vector<string>> Discret;
    int tot = 0;
    for (const auto& pr : Discretization) {
        tot++;
        int root = Findx(pr.second);
        Discret[root].push_back(pr.first);
    }

    // 输出结果到文件
    ofstream outFile("C:\\Users\\86183\\Desktop\\DblpFolder\\database\\auath_result.txt");
    if (!outFile)
    {
        cout << "1" << endl;
    }
    for (const auto& pr : Discret) {
        for (const auto& author : pr.second) {
            outFile << author << " " << Findx(Parallel[Discretization[author]]) << endl;
        }
    }
    outFile.close();

    // 统计作者数大于100的集合数目
    vector<int> aua;
    for (const auto& pr : Discret) {
        if (pr.second.size() >= 100) {
            aua.push_back(pr.first);
        }
    }



    sort(aua.begin(), aua.end(), [&](int x, int y) {
        return Discret[y].size() < Discret[x].size();
    });

    ofstream rankFile("C:\\Users\\86183\\Desktop\\DblpFolder\\database\\aua_rank_result.txt");
    if (!rankFile)
    {
        cout << "2" << endl;
    }
    rankFile << "作者数>100的聚团数目 " << aua.size() << "\n";
    for (int i : aua) {
        rankFile << i << " " << Discret[i].size() << endl;
    }
    rankFile.close();

    // 创建文件夹并保存结果
    string folderPath = "C:\\Users\\86183\\Desktop\\DblpFolder\\database\\aua_result\\";
    for (int i : aua) {
        ofstream clusterFile(folderPath + to_string(i) + ".txt");

        for (const auto& author : Discret[i]) {
            clusterFile << author << endl;
        }
        clusterFile.close();
    }

    return 0;
}
