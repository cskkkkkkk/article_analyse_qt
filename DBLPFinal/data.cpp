#include "data.h"


namespace data {
    const int MAX_NUM = 4096;//HASH最大值(请勿修改，由Hash4函数获得)
    size_t total_num = 0;//用于记录数据的总数量
    int alive_thread_initial = 0;//用于记录当前活动的线程数量。在多线程的环境下，用于控制并发线程的数量。
    bool sys_writing = false;//用于表示当前是否有线程正在执行写入操作。在多线程环境下，用于确保写入操作的互斥性。

    bool outtmpwzfl[MAX_NUM] = { false };
    bool outtmpzzfl[MAX_NUM] = { false };
    bool outtmpyearfl[3000] = { false };//用于表示对应的输出缓存是否正在被写入。
    out_string* outtmpwz[MAX_NUM] = { 0 };//outtmpwz 用于保存文章相关数据
    out_string* outtmpzz[MAX_NUM] = { 0 };//outtmpzz 用于保存作者相关数据
    out_string* outtmpyear[3000] = { 0 };//outtmpyear 用于保存年份相关数据。
    vector<string> auth[16];//用于保存作者信息。在解析数据时，会将作者信息存储到这个数组中。
    string quike_reader[16];//用于临时存储读取的内容。在解析数据时，会将读取的内容存储到这个数组中。
    DWORD quike_reader_p[16] = { 0 };//quike_reader 数组中各个位置的读取指针位置。在解析数据时，用于记录当前读取位置。

    class out_string { //输出内容缓存
    public:
        string out;
        out_string* next;
        out_string() {
            out = "";
            next = NULL;
        }
    };

//merge sort
    template<typename T> inline void sort(vector<T>& a, int s, int t) {
        if (s >= t)return;
        int l = s, r = t;
        T tmp = a[s];
        while (l < r) {
            while (l < r && a[r] >= tmp)r--;
            while (l < r && a[l] <= tmp)l++;
            if (l != r)swap(a[r], a[l]);
        }
        swap(a[s], a[r]);
        sort(a, s, l - 1);
        sort(a, l + 1, t);
        return;
    }


/*首先，它检查全局变量 sys_writing，如果为 true，表示当前有其他线程在写入数据，函数会进入循环等待直到 sys_writing 变为 false，以避免多个线程同时写入导致数据混乱。
然后，它检查数组 outtmpwzfl 中指定编号 num 的标志位是否为 true，表示该位置的输出缓存是否正在被写入。如果是，则进入循环等待，直到标志位变为 false。
设置 outtmpwzfl[num] 为 true，表示开始写入该编号对应的输出缓存。
将之前存储在 outtmpwz[num] 中的输出缓存保存到临时变量 tmp 中，然后创建一个新的 out_string 对象，存储传入的字符串 content，并将该对象的 next 指针指向之前保存的缓存。
将新创建的 out_string 对象存储到 outtmpwz[num] 中，表示将该字符串加入到指定编号的输出缓存中。
将 outtmpwzfl[num] 设置为 false，表示写入操作完成。
整个函数的目的是确保多线程写入数据时不会产生冲突，保证数据的完整性和一致性。*/


//写入文章相关数据
    inline void push_wz(string content, DWORD num) {
        while (sys_writing) { Sleep(1000); }//检查线程是否被写入
        while (outtmpwzfl[num]) {}
        outtmpwzfl[num] = true;
        out_string* tmp = outtmpwz[num];//存的是一个头指针
        outtmpwz[num] = new out_string;
        outtmpwz[num]->out = content;
        outtmpwz[num]->next = tmp;
        outtmpwzfl[num] = false;
        return;
    }




//写入年份相关数据
    inline void push_year(string content, DWORD num) {
        while (sys_writing) { Sleep(1000); }
        while (outtmpyearfl[num]) {}
        outtmpyearfl[num] = true;
        out_string* tmp = outtmpyear[num];
        outtmpyear[num] = new out_string;
        outtmpyear[num]->out = content;
        outtmpyear[num]->next = tmp;
        outtmpyearfl[num] = false;
        return;
    }



//写入作者相关数据，应该是解决那个，输入作者名展示所有和它有合作的人，这里存放的是一篇文章
//中所有作者名最后面再加上文章名字
    inline void push_author(string content, vector<string>& author) {
        while (sys_writing) { Sleep(1000); }
        string atmp = "";
        for (int i = 0; i < author.size(); i++) {
            atmp = atmp + "$" + author[i];
        }
        for (int i = 0; i < author.size(); i++) {
            while (outtmpzzfl[Hash4(author[i])]) {}
            outtmpzzfl[Hash4(author[i])] = true;
            out_string* tmp = outtmpzz[Hash4(author[i])];
            outtmpzz[Hash4(author[i])] = new out_string;
            outtmpzz[Hash4(author[i])]->out = atmp + "$" + content;
            outtmpzz[Hash4(author[i])]->next = tmp;
            outtmpzzfl[Hash4(author[i])] = false;
        }
        return;
    }




    inline DWORD Hash4(const string& tar) {
        DWORD64 ans = 0;
        for (int i = 0; i < tar.length(); i++) {
            ans = ((ans >> 8) & 0xf) ^ ((ans << 4) ^ tar[i]);
        }
        return ans & 0xfff;
    }




/*
首先检查是否已经读取完了当前存储在 quike_reader 中的字符串，或者 quike_reader 中没有字符串。如果是，则需要从文件流中读取下一行内容到 quike_reader 中。
如果 quike_reader[ID] 是空的，或者 quike_reader_p[ID] 指向的位置已经超过了 quike_reader[ID] 的长度，就表示当前存储在 quike_reader[ID] 中的字符串已经读取完了。
这时，程序尝试从文件流 iner 中读取一行内容，并将其存储到 quike_reader[ID] 中。
如果成功读取了一行内容，则将 quike_reader_p[ID] 重置为 0，表示将从新的一行内容中开始读取字符；否则，说明已经读取到了文件流的末尾，函数返回 false。
*/
//quike_reader是一个缓冲区,为空或者读取完毕都继续读一行然后读出一个字符，然后指针位置挪一位
    inline bool gotchar(fstream& iner, DWORD ID, char& outs) {
        if (quike_reader[ID] == "" || quike_reader_p[ID] >= quike_reader[ID].size()) {
            if (getline(iner, quike_reader[ID]))
                quike_reader_p[ID] = 0;
            else return false;
        }
        outs = quike_reader[ID][quike_reader_p[ID]];
        quike_reader_p[ID]++;
        return true;
    }


/*
总体来说，这个函数用于创建文章，并将相关信息存储到输出缓存中，包括文章名称、标志、作者列表和年份。在多线程环境
下，通过使用不同的 ID 来
确定应该将作者添加到哪个位置的 auth 数组中，以及将数据推送到哪个输出缓存中，从而实现了并发处理。
*/
    inline void Create_article(string name, DWORD flag, vector<string>& author, DWORD years, DWORD ID) {
        if (author.size() > 256) {//疑惑问题处理???
            DWORD nh = Hash4(name);
            push_wz(name + " <-> " + to_string(flag), nh);
            push_year(name, years);
            total_num++;
            return;
        }
        for (int i = 0; i < author.size(); i++)auth[ID].push_back(author[i]);
        DWORD nh = Hash4(name);
        push_wz(name + " <-> " + to_string(flag), nh);
        push_year(name, years);
        push_author(name, author);
        author.clear();
        total_num++;
        return;
    }





/*
总体来说，这个函数用于解析文件流中的记录，并根据记录中的信息调用 Create_article 函数创建文章记录。
在解析过程中，针对不同的标签进行相应的处理，确保记录信息能够正确地被提取和处理。
*/

/*
在 reader 函数中，flag 的赋值发生在发现一个新的标签起始符号 < 时，用于记录当前标签的起始位置。这个起始位置会在后续处理过程中被使用。

在处理完一个完整的标签后，调用 Write_Record 函数处理记录。flag 被传递给 Write_Record 函数，以指示需要处理的标签的起始位置。

在 Write_Record 函数中，如果发现了异常数据或者处理过程中需要记录位置时，flag 被返回，用于更新下一个需要处理的标签的起始位置。

*/
    inline DWORD Write_Record(fstream& iner, DWORD flag, string& tar, DWORD ID) {
        if (tar.find("<article") == string::npos && tar.find("<inproceedings") == string::npos && tar.find("<proceedings") == string::npos &&
            tar.find("<book") == string::npos && tar.find("<incollection") == string::npos && tar.find("<phdthesis") == string::npos &&
            tar.find("<mastersthesis") == string::npos && tar.find("<www") == string::npos && tar.find("<person") == string::npos &&
            tar.find("<data") == string::npos)//寻找起始地址
            return 0;
        char tmpc;
        string tmp;
        int kd = 0;
        vector<string> author;
        string name = "";
        author.clear();
        DWORD err_protection = 0;//非法数据格式兼容
        while (gotchar(iner, ID, tmpc)) {
            if (tmpc == '<' && kd == 0) {
                tmp = "<";
                kd = 1;
                err_protection = (DWORD)(iner.tellg()) + quike_reader_p[ID] - quike_reader[ID].length();
                continue;
            }
            else if (tmpc == '<' && kd > 0) {
                tmp = tmp + '<';
                kd++;
                continue;
            }
            else if (tmpc == '>' && kd > 0) {
                kd--;
                tmp = tmp + '>';
                if (kd <= 0) {
                    kd = 0;
                    if (tmp == "<author>" || tmp == "<editor>") {
                        tmp = "";
                        while (gotchar(iner, ID, tmpc)) {
                            if (tmpc != '<')tmp = tmp + tmpc;
                            else {
                                author.push_back(tmp);
                                break;
                            }
                        }
                    }
                    else if (tmp == "<title>" || tmp == "<booktitle>") {
                        tmp = "";
                        while (gotchar(iner, ID, tmpc)) {
                            if (tmpc != '<')tmp = tmp + tmpc;
                            else {
                                name = tmp;
                                break;
                            }
                        }
                    }
                    else if (tmp == "<year>") {//完成记录需要数据的处理
                        int years = 0;
                        while (gotchar(iner, ID, tmpc)) {
                            if (tmpc != ' ')break;
                        }
                        while (tmpc >= '0' && tmpc <= '9') {
                            years = years * 10 + tmpc - '0';
                            gotchar(iner, ID, tmpc);
                        }
                        Create_article(name, flag, author, years, ID);
                        break;
                    }
                }
            }
            else if (kd > 0) {
                tmp = tmp + tmpc;
                if (tar.find("<article") != string::npos && tar.find("<inproceedings") != string::npos && tar.find("<proceedings") != string::npos &&
                    tar.find("<book") != string::npos && tar.find("<incollection") != string::npos && tar.find("<phdthesis") != string::npos &&
                    tar.find("<mastersthesis") != string::npos && tar.find("<www") != string::npos && tar.find("<person") != string::npos &&
                    tar.find("<data") != string::npos)//未知错误兼容
                {
                    Create_article(name, flag, author, 1, ID);//错误数据强制储存
                    author.clear();
                    return err_protection;
                    //Write_Record(iner, err_protection, tmp, ID);//再处理
                    break;
                }
            }
        }
        return 0;
    }



    void reader(fstream& iner, DWORD offset, DWORD Terminal, DWORD ID) {
#ifdef DEBUGING
        cout << "线程" << ID << "反馈正常" << endl;
#endif // DEBUGING
        iner.seekg(offset);
        char tmpc;//用于临时存储每次读取的字符
        string tmp;//用于临时存储正在处理的字符串
        int kd = 0;//用于指示当前是否处于标签内部
        DWORD flag = 0;//当前偏移量
        DWORD position = (DWORD)(iner.tellg()) + quike_reader_p[ID] - quike_reader[ID].length();
        while (position <= Terminal + 1 && gotchar(iner, ID, tmpc)) {
            if (tmpc == '<' && kd == 0) {
                tmp = "<";
                kd = 1;
                flag = (DWORD)(iner.tellg()) + quike_reader_p[ID] - quike_reader[ID].length();
                continue;
            }
            else if (tmpc == '<' && kd > 0) {
                tmp = tmp + '<';
                kd++;
                continue;
            }
            else if (tmpc == '>' && kd > 0) {
                kd--;
                tmp = tmp + '>';
                if (kd <= 0) {
                    kd = 0;
                    while (flag) {
                        flag = Write_Record(iner, flag, tmp, ID);//异常数据兼容
                    }
                }
            }
            else if (kd > 0) {
                tmp = tmp + tmpc;
            }
            position = (DWORD)(iner.tellg()) + quike_reader_p[ID] - quike_reader[ID].length();
        }
        while (flag) {
            //处理完一个完整的标签后，调用 Write_Record 函数处理记录。该函数将返回下一个需要处理的偏移量，直到没有待处理的标签。
            flag = Write_Record(iner, flag, tmp, ID);//异常数据兼容
        }
        alive_thread_initial--;



#ifdef DEBUGING
        cout << "线程" << ID << "结束运行" << endl;
#endif // DEBUGING



        return;
    }





    string _FileUrl_All;//_FileUrl_All 变量是一个字符串类型的变量，用于存储文件的路径或文件夹的路径。



//把数据写入到文件中
    inline void do_writer() {
        //遍历 outtmpwz 数组，将缓存在 outtmpwz 中的文章数据写入对应的文件中。
        for (int i = 0; i < MAX_NUM; i++) {
            if (outtmpwz[i] == NULL)continue;
            fstream ot(_FileUrl_All + "database\\article\\" + to_string(i) + ".ini", ios::app);
            out_string* now = outtmpwz[i];
            outtmpwz[i] = NULL;
            while (now != NULL) {
                ot << (now->out) << endl;
                out_string* tmp = now;
                now = now->next;
                delete tmp;
            }
            ot.close();
        }
        //遍历 outtmpzz 数组，将缓存在 outtmpzz 中的作者数据写入对应的文件中。
        for (int i = 0; i < MAX_NUM; i++) {
            if (outtmpzz[i] == NULL)continue;
            fstream ot(_FileUrl_All + "database\\author\\" + to_string(i) + ".ini", ios::app);
            out_string* now = outtmpzz[i];
            outtmpzz[i] = NULL;
            while (now != NULL) {
                ot << (now->out) << endl;
                out_string* tmp = now;
                now = now->next;
                delete tmp;
            }
            ot.close();
        }
        //遍历 outtmpyear 数组，将缓存在 outtmpyear 中的年份数据写入对应的文件中。
        for (int i = 0; i < 3000; i++) {
            if (outtmpyear[i] == NULL)continue;
            fstream ot(_FileUrl_All + "database\\year\\" + to_string(i) + ".ini", ios::app);
            out_string* now = outtmpyear[i];
            outtmpyear[i] = NULL;
            while (now != NULL) {
                ot << (now->out) << endl;
                out_string* tmp = now;
                now = now->next;
                delete tmp;
            }
            ot.close();
        }
//合并其他线程中的作者数据到主线程的作者数据中，并将其写入文件中。
        for (int i = 1; i < 16; i++) {
            for (int j = 0; j < auth[i].size(); j++) {
                auth[0].push_back(auth[i][j]);
            }
            auth[i].clear();
        }
        //合并其他线程中的作者数据到主线程的作者数据中，并将其写入文件中。
        fstream allauthor(_FileUrl_All + "database\\author_rank", ios::app);
        sort(auth[0], 0, auth[0].size() - 1);
        string tmpau = auth[0][0];//当前作者
        int tot = 0;//当前作者的出现次数
        for (int i = 0; i < auth[0].size(); i++) {
            if (auth[0][i] != tmpau) {
                allauthor << tot << " " << tmpau << endl;
                tmpau = auth[0][i];
                tot = 1;
            }
            else {
                tot++;
            }
        }
        allauthor.close();
        auth[0].clear();
    }

    bool writing_finished = false;


//总的来说，writer() 函数的作用是周期性地将缓存的数据写入到文件中，以保证数据的持久化存储。这是一个输出线程
   void writer() {
        int mytmp = 0;//int mytmp = 0;: 初始化一个变量 mytmp，用于跟踪上一次写入的总数。
        while (!writing_finished) {
            if (9 - mytmp >= 1000000) {


#ifdef DEBUGING
                cout << "控制线程申请输出." << endl;
#endif // DEBUGING


                sys_writing = true;
                mytmp = total_num;
                do_writer();
                sys_writing = false;
            }
#ifdef DEBUGING
            cout << "控制线程正在运行" << endl;
            Sleep(3000);
#else

            Sleep(500);
#endif // DEBUGING
        }


#ifdef DEBUGING
        cout << "控制线程申请注销." << endl;
#endif // DEBUGING

//这段代码的作用是确保在 while 循环结束后，即使没有达到数据输出的条件，也能够最终将剩余的数据进行输出。
        sys_writing = true;
        mytmp = total_num;
        do_writer();
        sys_writing = false;
#ifdef DEBUGING
        cout << "控制线程已注销." << endl;
#endif // DEBUGING
        return;
    }


/*
第一个参数 CP_ACP 指定了源字符串的字符编码方式。CP_ACP 是 Windows 默认的 ANSI 代码页，表示使用当前系统的 ANSI 字符编码。
第二个参数是一个标志位，一般情况下可以设为 0，表示使用默认行为。
第三个参数是源字符串的指针，是一个 LPCSTR 类型，表示一个指向以 null 结尾的多字节字符的常量指针。在这里，str.c_str() 返回的是一个指向 std::string 对象内部字符数组的指针，类型为 const char*，通过强制类型转换 (LPCSTR) 将其转换为 LPCSTR 类型。
第四个参数是源字符串的长度，如果设置为 -1，则表示源字符串是以 null 结尾的，函数会自动计算字符串的长度。
第五个参数是目标缓冲区的指针，是一个 LPWSTR 类型，表示一个指向宽字符数组的指针，用于存储转换后的宽字符。
第六个参数是目标缓冲区的大小，以字符数表示。
*/
    LPCWSTR stringtolstr(string str) {
        LPWSTR ans = new WCHAR[str.length() + 1];
        MultiByteToWideChar(CP_ACP, 0, (LPCSTR)str.c_str(), -1, ans, str.length());
        return ans;
    }



    bool initial_readers(DWORD Max_thread, DWORD TOTAL_THREAD, bool File_check, string _FileUrl) {
        _FileUrl_All = (string)_FileUrl;
        //CreateFile 函数：这是 Windows API 中用于创建或打开文件的函数。它的参数依次是：

        HANDLE check_file_handle = CreateFile(stringtolstr((string)_FileUrl + "database\\finish.db"), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
        //INVALID_HANDLE_VALUE 是 Windows API 中定义的一个常量，用于表示无效的句柄值。在调用 CreateFile 函数时，如果该函数未能成功创建或打开文件，它将返回 INVALID_HANDLE_VALUE。
        if (File_check && check_file_handle != INVALID_HANDLE_VALUE) {
            cout << "已存在完成的数据库构建";
            return true;
        }
        if (Max_thread > 16)return false;
        //被激活的线程为0说明目前没有可用的线程
        alive_thread_initial = 0;
        string paths = "mkdir " + (string)_FileUrl + "database\\author 2>nul";
        char* pathss = new char[paths.length() + 1];
        for (int i = 0; i < paths.length(); i++) {
            pathss[i] = paths[i];
        }
        pathss[paths.length()] = '\0';
        system(pathss);
#ifdef DEBUGING
        cout << pathss << endl;
#endif // DEBUGING
        paths = "mkdir " + (string)_FileUrl + "database\\article 2>nul";
        pathss = new char[paths.length() + 1];
        for (int i = 0; i < paths.length(); i++) {
            pathss[i] = paths[i];
        }
        pathss[paths.length()] = '\0';
        system(pathss);
#ifdef DEBUGING
        cout << pathss << endl;
#endif // DEBUGING
        paths = "mkdir " + (string)_FileUrl + "database\\year 2>nul";
        pathss = new char[paths.length() + 1];
        for (int i = 0; i < paths.length(); i++) {
            pathss[i] = paths[i];
        }
        pathss[paths.length()] = '\0';
        system(pathss);
#ifdef DEBUGING
        cout << pathss << endl;
#endif // DEBUGING


//到这里上面创建了四个文件夹



//开始创建了16个线程
        thread* readers[16];
        fstream* infile = new fstream[Max_thread];

        //依次打开每个线程所需要的文件流对象
        for (int i = 0; i < Max_thread; i++) {
            infile[i].open((string)_FileUrl + "dblp.xml", ios::in);
            if (!infile[i]) {
                cout << "文件流" << i << "未能打开文件" << (string)_FileUrl + "dblp.xml" << endl;
                return false;
            }
        }

        DWORD64 filesize = 0;

        HANDLE target_file_handle = CreateFile(stringtolstr((string)_FileUrl + "dblp.xml"), FILE_READ_EA, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
        filesize = GetFileSize(target_file_handle, NULL);//打开dblp文件获取大小


#ifdef DEBUGING
        cout << "File Size:" << filesize << endl;
#endif // DEBUGING


        DWORD64 offset = filesize / Max_thread;


#ifdef DEBUGING
        cout << "创建控制线程中" << endl;
#endif // DEBUGING

//writers(writer): 这行代码创建了一个名为 writers 的线程对象，并将函数 writer 作为参数传递给了该线程对象。这意味着在启动 writers 线程时，将执行 writer 函数中的代码。这是输出线程
//初始化的是输出线程，并且执行输出的函数
        thread writers(writer);


#ifdef DEBUGING
        cout << "已创建控制线程" << endl;
#endif // DEBUGING

//把起始位置移动到文件最开头
        infile[Max_thread - 1].seekg(0, ios::beg);



#ifdef DEBUGING
        cout << "共需要创建线程数:" << Max_thread << endl;
#endif // DEBUGING






        for (int i = 0; i < Max_thread - 1; i++) {
#ifdef DEBUGING
            cout << "正在创建线程" << i << endl;
#endif // DEBUGING
        //记录开始的位置
            DWORD startpoi = infile[Max_thread - 1].tellg();

            infile[Max_thread - 1].seekg((i + 1) * offset);
            char tmpchar;
            while (tmpchar = infile[Max_thread - 1].get()) {
                if (tmpchar == '>') {
                    //记录下'>'的位置
                    DWORD tmpe = infile[Max_thread - 1].tellg();
                    //激活一个线程
                    alive_thread_initial++;
                    //readers是存放线程的数组
                    //reader是一个函数
                    //startpoi这些参数作为对象传给reader这个函数
                    (readers[i]) = new thread(reader, ref(infile[i]), startpoi, tmpe, i);
#ifdef DEBUGING
                    cout << "已创建线程" << i << endl;
#endif // DEBUGING

//这段代码的作用是在当前线程等待，直到活动的初始读取线程数量 alive_thread_initial 小于总线程数 TOTAL_THREAD 为止。
                    while (alive_thread_initial >= TOTAL_THREAD) {
                        if (!sys_writing)cout << "读取中:" << total_num << endl;
                        else cout << "写入中:" << total_num << endl;
                        Sleep(1000);
                    }
                    break;
                }


            }


        }


//现在处理最后一个线程标号为max_thread-1的线程
        alive_thread_initial++;
        (readers[Max_thread - 1]) = new thread(reader, ref(infile[Max_thread - 1]), (DWORD)infile[Max_thread - 1].tellg(), filesize - 1, Max_thread - 1);


#ifdef DEBUGING
        cout << "Create Thread " << Max_thread - 1 << endl;
#endif // DEBUGING


        while (alive_thread_initial) {
            if (!sys_writing)cout << "读取中:" << total_num << endl;
            else cout<<"写入中:" << total_num << endl;
            Sleep(1000);
        }
        cout << "已完成读取:" << total_num << endl;
        writing_finished = true;
        //关闭输出线程
        writers.join();
        cout << "已完成数据写入" << endl;
        fstream finishes((string)_FileUrl + "database\\finish.db", ios::out);
        finishes << total_num;
        finishes.close();
        return true;
    }
}
